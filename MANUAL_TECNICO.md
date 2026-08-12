# Manual Técnico - Analizador Léxico y Sintáctico EXT2

## Tabla de Contenidos
1. Arquitectura del Sistema
2. Estructuras Utilizadas
3. Análisis Léxico
4. Análisis Sintáctico
5. Comandos Implementados
6. Compilación e Instalación
7. Ejemplos de Uso

---

## 1. Arquitectura del Sistema

El proyecto utiliza una arquitectura de dos capas:

### Backend (C++)
- Responsabilidad: Análisis léxico y sintáctico
- Ubicación: `/backend`
- Tecnología: C++17, sin dependencias externas
- Entrada: Comandos por consola
- Salida: Validación y errores

### Frontend (React)
- Responsabilidad: Interfaz de usuario
- Ubicación: `/frontend`
- Tecnología: React 18, Vite
- Porta: 5173
- Incluye: Réplica del analizador en JavaScript

### Flujo de Datos

```
Usuario Input
    ↓
Lexer (Tokenización)
    ↓
Parser (Validación)
    ↓
Resultado (Éxito/Errores)
    ↓
Visualización al usuario
```

---

## 2. Estructuras Utilizadas

### Estructura Token

```cpp
struct Token {
    std::string tipo;   // COMANDO, PARAMETRO, VALOR
    std::string valor;  // Contenido del token
};
```

**Uso**: Representa unidades mínimas de la entrada después de la tokenización.

### Estructura ResultadoLexico

```cpp
struct ResultadoLexico {
    bool exito;
    std::vector<Token> tokens;
    std::string error;
};
```

**Uso**: Almacena resultado del análisis léxico con tokens o mensaje de error.

### Estructura ResultadoSintactico

```cpp
struct ResultadoSintactico {
    bool valido;
    std::string comando;
    std::map<std::string, std::string> parametros;
    std::vector<std::string> errores;
};
```

**Uso**: Contiene resultado de validación sintáctica con parámetros encontrados y errores.

---

## 3. Análisis Léxico

### Responsabilidad
Descomponer entrada en tokens válidos y detectar errores de sintaxis básica.

### Implementación (lexer.cpp)

```cpp
ResultadoLexico Lexer::analizar(const std::string& entrada) {
    ResultadoLexico resultado{true, {}, ""};
    
    std::istringstream ss(entrada);
    std::string palabra;
    bool primerPalabra = true;

    std::vector<std::string> comandosValidos = {
        "MKDISK", "RMDISK", "FDISK", "MOUNT", "MKFS", 
        "MKUSR", "RMUSR", "MKFILE"
    };

    std::vector<std::string> parametrosValidos = {
        "-size", "-fit", "-unit", "-path", "-type", "-name", "-id",
        "-user", "-pass", "-grp", "-r", "-cont"
    };

    while (ss >> palabra) {
        if (primerPalabra) {
            std::string upper = palabra;
            std::transform(upper.begin(), upper.end(), 
                         upper.begin(), ::toupper);

            if (std::find(comandosValidos.begin(), 
                         comandosValidos.end(), upper) == 
                comandosValidos.end()) {
                resultado.exito = false;
                resultado.error = "Comando no valido: " + palabra;
                return resultado;
            }

            resultado.tokens.push_back({"COMANDO", upper});
            primerPalabra = false;
        }
        else {
            // Procesar parametros
            if (palabra[0] == '-') {
                size_t posIgual = palabra.find('=');
                std::string nombreParam = posIgual != std::string::npos ?
                    palabra.substr(0, posIgual) : palabra;

                std::transform(nombreParam.begin(), nombreParam.end(), 
                             nombreParam.begin(), ::tolower);

                if (std::find(parametrosValidos.begin(), 
                             parametrosValidos.end(), nombreParam) == 
                    parametrosValidos.end()) {
                    resultado.exito = false;
                    resultado.error = "Parametro invalido: " + nombreParam;
                    return resultado;
                }

                resultado.tokens.push_back({"PARAMETRO", nombreParam});

                if (posIgual != std::string::npos) {
                    std::string valor = palabra.substr(posIgual + 1);
                    resultado.tokens.push_back({"VALOR", valor});
                }
            } else {
                resultado.tokens.push_back({"VALOR", palabra});
            }
        }
    }

    return resultado;
}
```

### Validaciones
- Comando debe estar en lista permitida
- Parámetros deben comenzar con "-"
- Parámetro debe estar registrado
- Se preservan mayúsculas en valores

### Ejemplo

```
Entrada: mkdisk -size=100 -path=/disco.mia

Tokens generados:
  [COMANDO:MKDISK]
  [PARAMETRO:-size]
  [VALOR:100]
  [PARAMETRO:-path]
  [VALOR:/disco.mia]
```

---

## 4. Análisis Sintáctico

### Responsabilidad
Validar estructura de comandos y parámetros según reglas específicas.

### Implementación Base (parser.cpp)

```cpp
ResultadoSintactico Parser::analizar(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        return {false, "", {}, {"Sin tokens para analizar"}};
    }

    std::string comando = tokens[0].valor;
    std::map<std::string, std::string> parametros;

    // Extraer parametros
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].tipo == "PARAMETRO") {
            std::string paramName = tokens[i].valor;
            std::string valor = "";
            if (i + 1 < tokens.size() && tokens[i + 1].tipo == "VALOR") {
                valor = tokens[i + 1].valor;
                i++;
            }
            parametros[paramName] = valor;
        }
    }

    // Validar según comando
    if (comando == "MKDISK") return validarMKDISK(parametros);
    if (comando == "RMDISK") return validarRMDISK(parametros);
    if (comando == "FDISK") return validarFDISK(parametros);
    if (comando == "MOUNT") return validarMOUNT(parametros);
    if (comando == "MKFS") return validarMKFS(parametros);
    if (comando == "MKUSR") return validarMKUSR(parametros);
    if (comando == "RMUSR") return validarRMUSR(parametros);
    if (comando == "MKFILE") return validarMKFILE(parametros);

    return {false, comando, parametros, {"Comando desconocido"}};
}
```

### Ejemplo de Validación (MKDISK)

```cpp
ResultadoSintactico Parser::validarMKDISK(
    const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MKDISK", params, {}};

    if (params.find("-size") == params.end()) {
        resultado.errores.push_back(
            "Falta parametro obligatorio: -size");
        resultado.valido = false;
    } else {
        try {
            int size = std::stoi(params.at("-size"));
            if (size <= 0) {
                resultado.errores.push_back(
                    "El tamanio debe ser positivo");
                resultado.valido = false;
            }
        } catch (...) {
            resultado.errores.push_back(
                "-size debe ser un numero");
            resultado.valido = false;
        }
    }

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back(
            "Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    auto unitIt = params.find("-unit");
    if (unitIt != params.end()) {
        std::string unit = unitIt->second;
        if (unit != "K" && unit != "M") {
            resultado.errores.push_back(
                "Unidad invalida. Debe ser K o M");
            resultado.valido = false;
        }
    }

    return resultado;
}
```

---

## 5. Comandos Implementados

### MKDISK - Crear Disco Virtual

**Parámetros**:
- `-size` (obligatorio): Tamaño del disco (número positivo)
- `-path` (obligatorio): Ruta del archivo
- `-unit` (opcional): K (Kilobytes) o M (Megabytes), default M
- `-fit` (opcional): BF, FF o WF, default FF

**Validaciones**:
- size debe ser número positivo
- unit debe ser K o M
- fit debe ser BF, FF o WF

**Ejemplo**:
```bash
mkdisk -size=100 -path=/disco.mia
mkdisk -size=50 -unit=K -path=/disco2.mia
```

### RMDISK - Eliminar Disco

**Parámetros**:
- `-path` (obligatorio): Ruta del disco a eliminar

**Ejemplo**:
```bash
rmdisk -path=/disco.mia
```

### FDISK - Administrar Particiones

**Parámetros**:
- `-size` (obligatorio): Tamaño de partición (número positivo)
- `-path` (obligatorio): Ruta del disco
- `-name` (obligatorio): Nombre de la partición
- `-unit` (opcional): B, K o M, default K
- `-type` (opcional): P (primaria), E (extendida), L (lógica), default P
- `-fit` (opcional): BF, FF o WF, default FF

**Validaciones**:
- size debe ser número positivo
- unit debe ser B, K o M
- type debe ser P, E o L
- fit debe ser BF, FF o WF

**Ejemplo**:
```bash
fdisk -size=50 -path=/disco.mia -name=Particion1
fdisk -size=100 -unit=M -path=/disco.mia -name=Part2 -type=E
```

### MOUNT - Montar Partición

**Parámetros**:
- `-path` (obligatorio): Ruta del disco
- `-name` (obligatorio): Nombre de la partición

**Ejemplo**:
```bash
mount -path=/disco.mia -name=Particion1
```

### MKFS - Formatear Partición

**Parámetros**:
- `-id` (obligatorio): ID de la partición montada
- `-type` (opcional): "full" para formateo completo, default full

**Ejemplo**:
```bash
mkfs -id=341A
mkfs -id=342A -type=full
```

### MKUSR - Crear Usuario

**Parámetros**:
- `-user` (obligatorio): Nombre usuario (máximo 10 caracteres)
- `-pass` (obligatorio): Contraseña (máximo 10 caracteres)
- `-grp` (obligatorio): Grupo (máximo 10 caracteres)

**Validaciones**:
- Cada parámetro máximo 10 caracteres

**Ejemplo**:
```bash
mkusr -user=juan -pass=pass123 -grp=admin
```

### RMUSR - Eliminar Usuario

**Parámetros**:
- `-user` (obligatorio): Nombre del usuario a eliminar

**Ejemplo**:
```bash
rmusr -user=juan
```

### MKFILE - Crear Archivo

**Parámetros**:
- `-path` (obligatorio): Ruta del archivo
- `-size` (opcional): Tamaño en bytes (número >= 0), default 0
- `-r` (opcional): Flag para crear directorios si no existen
- `-cont` (opcional): Archivo origen para copiar contenido

**Validaciones**:
- size no puede ser negativo

**Ejemplo**:
```bash
mkfile -path=/archivo.txt -size=100
mkfile -path=/archivo.txt -size=50 -r
mkfile -path=/archivo.txt -cont=/home/archivo.txt
```

---

## 6. Compilación e Instalación

### Requisitos
- MSYS2 (con g++ incluido)
- Node.js 18+ (para frontend)
- CMake (opcional)

### Compilar Backend

```bash
cd backend
g++ -std=c++17 src/main.cpp src/lexer.cpp src/parser.cpp -o analizador.exe
./analizador.exe
```

### Instalar y Ejecutar Frontend

```bash
cd frontend
npm install
npm run dev
```

Acceder a `http://localhost:5173` en navegador.

---

## 7. Ejemplos de Uso

### Caso 1: Comando Válido

```
Input:  mkdisk -size=100 -path=/disco.mia
Output: TOKENS: [COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:100] [PARAMETRO:-path] [VALOR:/disco.mia]
        COMANDO VALIDO
```

### Caso 2: Parámetro Faltante

```
Input:  mkdisk -size=100
Output: TOKENS: [COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:100]
        ERRORES:
          - Falta parametro obligatorio: -path
```

### Caso 3: Valor Inválido

```
Input:  mkdisk -size=abc -path=/disco.mia
Output: TOKENS: [COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:abc] [PARAMETRO:-path] [VALOR:/disco.mia]
        ERRORES:
          - -size debe ser un numero
```

### Caso 4: Parámetro No Reconocido

```
Input:  mkdisk -size=100 -path=/disco.mia -invalid=test
Output: ERROR LEXICO: Parametro invalido: -invalid
```

### Caso 5: Comando No Válido

```
Input:  mkdisk2 -size=100 -path=/disco.mia
Output: ERROR LEXICO: Comando invalido: mkdisk2
```
