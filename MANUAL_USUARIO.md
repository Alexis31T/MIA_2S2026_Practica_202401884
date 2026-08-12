# Manual de Usuario - Analizador EXT2

## Introducción

El Analizador Léxico y Sintáctico EXT2 es una herramienta web que valida comandos de administración de discos antes de ejecutarlos. No modifica archivos reales, solo verifica que los comandos estén correctamente escritos.

## Cómo Iniciar

### Opción 1: Frontend Web (Recomendado)

```bash
cd frontend
npm install
npm run dev
```

Luego abre `http://localhost:5173` en tu navegador.

### Opción 2: Backend por Consola

```bash
cd backend
g++ -std=c++17 src/main.cpp src/lexer.cpp src/parser.cpp -o analizador.exe
./analizador.exe
```

---

## Interfaz Web

La aplicación web tiene tres secciones principales:

### 1. Área de Entrada
- **Campo de texto**: Ingresa tu comando EXT2
- **Botón "Analizar"**: Valida el comando
- **Botón "Limpiar"**: Limpia los campos

### 2. Análisis Léxico
Muestra los tokens encontrados en el comando (verde = válido):

```
[COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:50] [PARAMETRO:-unit] [VALOR:K] [PARAMETRO:-path] [VALOR:/disco2.mia]
```

### 3. Análisis Sintáctico
Indica si el comando es válido o muestra errores encontrados:

- ✓ **Verde**: Comando válido
- ✗ **Rojo**: Errores en validación

---

## Ejemplos de Uso

### Ejemplo 1: Comando Válido

**Input**: `mkdisk -size=50 -unit=K -path=/disco2.mia`

**Resultado**:
```
Análisis Léxico:
[COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:50] [PARAMETRO:-unit] [VALOR:K] [PARAMETRO:-path] [VALOR:/disco2.mia]

Análisis Sintáctico:
✓ Comando válido
```

### Ejemplo 2: Comando con Error

**Input**: `mkdisk -size=abc -path=/disco.mia`

**Resultado**:
```
Análisis Léxico:
[COMANDO:MKDISK] [PARAMETRO:-size] [VALOR:abc] [PARAMETRO:-path] [VALOR:/disco.mia]

Análisis Sintáctico:
✗ -size debe ser positivo
```

### Ejemplo 3: Parámetro Faltante

**Input**: `fdisk -size=100 -path=/disco.mia`

**Resultado**:
```
Análisis Sintáctico:
✗ Falta parametro obligatorio: -name
```

### Ejemplo 4: Comando con Múltiples Parámetros

**Input**: `mkfile -path=/archivo.txt -size=100`

**Resultado**:
```
Análisis Léxico:
[COMANDO:MKFILE] [PARAMETRO:-path] [VALOR:/archivo.txt] [PARAMETRO:-size] [VALOR:100]

Análisis Sintáctico:
✓ Comando válido
```

---

## Comandos Disponibles

### MKDISK - Crear Disco

Crea un disco virtual (archivo binario simulado).

**Sintaxis**: `mkdisk -size=NUMERO -path=RUTA [-unit=UNIDAD] [-fit=AJUSTE]`

**Ejemplos**:
```bash
mkdisk -size=100 -path=/disco.mia
mkdisk -size=50 -unit=K -path=/disco2.mia
mkdisk -size=5 -unit=M -path=/disco3.mia -fit=BF
```

**Parámetros**:
- `-size` (obligatorio): Número positivo
- `-path` (obligatorio): Ruta del archivo
- `-unit` (opcional): K (Kilobytes) o M (Megabytes), default M
- `-fit` (opcional): BF, FF o WF, default FF

---

### RMDISK - Eliminar Disco

Elimina un archivo que representa un disco.

**Sintaxis**: `rmdisk -path=RUTA`

**Ejemplos**:
```bash
rmdisk -path=/disco.mia
rmdisk -path=/disco2.mia
```

**Parámetros**:
- `-path` (obligatorio): Ruta del disco a eliminar

---

### FDISK - Administrar Particiones

Crea o modifica particiones dentro de un disco.

**Sintaxis**: `fdisk -size=NUMERO -path=RUTA -name=NOMBRE [-unit=UNIDAD] [-type=TIPO] [-fit=AJUSTE]`

**Ejemplos**:
```bash
fdisk -size=50 -path=/disco.mia -name=Particion1
fdisk -size=100 -unit=M -path=/disco.mia -name=Part2 -type=E
fdisk -size=30 -path=/disco.mia -name=Part3 -type=L -fit=BF
```

**Parámetros**:
- `-size` (obligatorio): Número positivo
- `-path` (obligatorio): Ruta del disco
- `-name` (obligatorio): Nombre de la partición
- `-unit` (opcional): B, K o M, default K
- `-type` (opcional): P (primaria), E (extendida), L (lógica), default P
- `-fit` (opcional): BF, FF o WF

---

### MOUNT - Montar Partición

Monta una partición en el sistema.

**Sintaxis**: `mount -path=RUTA -name=NOMBRE`

**Ejemplos**:
```bash
mount -path=/disco.mia -name=Particion1
mount -path=/disco2.mia -name=Part2
```

**Parámetros**:
- `-path` (obligatorio): Ruta del disco
- `-name` (obligatorio): Nombre de la partición

---

### MKFS - Formatear Partición

Formatea una partición montada.

**Sintaxis**: `mkfs -id=ID_PARTICION [-type=TIPO]`

**Ejemplos**:
```bash
mkfs -id=341A
mkfs -id=342A -type=full
```

**Parámetros**:
- `-id` (obligatorio): ID de la partición montada
- `-type` (opcional): "full" para formateo completo, default full

---

### MKUSR - Crear Usuario

Crea un usuario en una partición.

**Sintaxis**: `mkusr -user=NOMBRE -pass=CONTRASEÑA -grp=GRUPO`

**Ejemplos**:
```bash
mkusr -user=juan -pass=pass123 -grp=admin
mkusr -user=maria -pass=1234 -grp=usuarios
```

**Parámetros**:
- `-user` (obligatorio): Máximo 10 caracteres
- `-pass` (obligatorio): Máximo 10 caracteres
- `-grp` (obligatorio): Máximo 10 caracteres

---

### RMUSR - Eliminar Usuario

Elimina un usuario de una partición.

**Sintaxis**: `rmusr -user=NOMBRE`

**Ejemplos**:
```bash
rmusr -user=juan
rmusr -user=maria
```

**Parámetros**:
- `-user` (obligatorio): Nombre del usuario a eliminar

---

### MKFILE - Crear Archivo

Crea un archivo dentro de una partición.

**Sintaxis**: `mkfile -path=RUTA [-size=TAMAÑO] [-r] [-cont=ARCHIVO_ORIGEN]`

**Ejemplos**:
```bash
mkfile -path=/archivo.txt -size=100
mkfile -path=/archivo.txt -size=50 -r
mkfile -path=/archivo.txt -cont=/home/archivo.txt
```

**Parámetros**:
- `-path` (obligatorio): Ruta del archivo
- `-size` (opcional): Tamaño en bytes, default 0
- `-r` (opcional): Crea directorios si no existen
- `-cont` (opcional): Archivo origen para copiar contenido

---

## Interpretación de Resultados

### Comando Válido
```
✓ Comando válido
```
El comando está correctamente escrito y todos los parámetros son válidos.

### Errores Comunes

**Error: Comando no válido**
```
ERROR LEXICO: Comando invalido: mkdisk2
```
El comando no existe. Verifica la ortografía.

**Error: Parámetro faltante**
```
✗ Falta parametro obligatorio: -path
```
Falta un parámetro obligatorio para este comando.

**Error: Valor inválido**
```
✗ -size debe ser positivo
```
El valor del parámetro no es válido.

**Error: Parámetro no reconocido**
```
ERROR LEXICO: Parametro invalido: -invalid
```
El parámetro no existe o está mal escrito.

---

## Consejos de Uso

1. **Verifica la ortografía**: Los comandos deben estar en mayúsculas o minúsculas (se convierten automáticamente)

2. **Parámetros con espacios**: Si la ruta tiene espacios, enciérrala en comillas:
   ```bash
   mkdisk -size=100 -path="/home/mis discos/disco.mia"
   ```

3. **Orden de parámetros**: No importa el orden de los parámetros:
   ```bash
   mkdisk -size=100 -path=/disco.mia
   mkdisk -path=/disco.mia -size=100
   ```

4. **Parámetros opcionales**: No es necesario incluir todos. El sistema usa valores por defecto:
   ```bash
   mkdisk -size=100 -path=/disco.mia  # usa -unit=M y -fit=FF por defecto
   ```

5. **Valores unitarios**: K = Kilobytes, M = Megabytes, B = Bytes

---

## Solución de Problemas

**P: ¿Por qué dice "Comando inválido" si lo escribí bien?**
R: Verifica que el comando sea uno de los 8 soportados: MKDISK, RMDISK, FDISK, MOUNT, MKFS, MKUSR, RMUSR, MKFILE.

**P: ¿Puedo usar mayúsculas y minúsculas?**
R: Sí, ambas funcionan. El sistema las normaliza automáticamente.

**P: ¿Modifica archivos reales?**
R: No. El analizador solo valida la sintaxis. No ejecuta ninguna operación.

**P: ¿Puedo usar sin el frontend?**
R: Sí, compila el backend en C++ y usa la consola.

---

## Capturas de Pantalla

### Pantalla Principal
![Interfaz principal](capturas/MIA1_Captura%201.png)

La interfaz muestra:
- Campo de entrada superior para ingresar comandos
- Botones "Analizar" y "Limpiar"
- Área de resultados debajo con análisis léxico y sintáctico

### Comando Válido - MKDISK
![Comando válido MKDISK](capturas/MIA1_Captura%202.png)

Input: `mkdisk -size=50 -unit=K -path=/disco2.mia`

**Resultado**:
- Análisis Léxico (verde): Muestra todos los tokens correctamente identificados
- Análisis Sintáctico (verde): ✓ Comando válido

### Comando con Error de Validación
![Comando con error](capturas/MIA1_Captura%203.png)

Input: `mkdisk -size=abc -path=/disco.mia`

**Resultado**:
- Análisis Léxico: Identifica los tokens
- Análisis Sintáctico (rojo): ✗ -size debe ser positivo

### Comando MKFILE Válido
![Comando válido MKFILE](capturas/MIA1_Captura%204.png)

Input: `mkfile -path=/archivo.txt -size=100`

**Resultado**:
- Análisis Léxico (verde): Tokens correctos
- Análisis Sintáctico (verde): ✓ Comando válido

### Esquema de Colores
- **Verde**: Comando válido, tokens correctos
- **Rojo**: Errores en la validación
- **Cyan/Azul**: Títulos y etiquetas
- **Naranja**: Resalte de parámetros y valores
