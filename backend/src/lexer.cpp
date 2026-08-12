#include "lexer.h"
#include <algorithm>
#include <sstream>
#include <cctype>

ResultadoLexico Lexer::analizar(const std::string& entrada) {
    ResultadoLexico resultado{true, {}, ""};

    std::istringstream ss(entrada);
    std::string palabra;
    bool primerPalabra = true;

    std::vector<std::string> comandosValidos = {
        "MKDISK", "RMDISK", "FDISK", "MOUNT", "MKFS", "MKUSR", "RMUSR", "MKFILE"
    };

    std::vector<std::string> parametrosValidos = {
        "-size", "-fit", "-unit", "-path", "-type", "-name", "-id",
        "-user", "-pass", "-grp", "-r", "-cont"
    };

    while (ss >> palabra) {
        if (primerPalabra) {
            // Primer token debe ser comando
            std::string upper = palabra;
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

            if (std::find(comandosValidos.begin(), comandosValidos.end(), upper) == comandosValidos.end()) {
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

                std::transform(nombreParam.begin(), nombreParam.end(), nombreParam.begin(), ::tolower);

                if (std::find(parametrosValidos.begin(), parametrosValidos.end(), nombreParam) == parametrosValidos.end()) {
                    resultado.exito = false;
                    resultado.error = "Parametro invalido: " + nombreParam;
                    return resultado;
                }

                resultado.tokens.push_back({"PARAMETRO", nombreParam});

                if (posIgual != std::string::npos) {
                    std::string valor = palabra.substr(posIgual + 1);
                    resultado.tokens.push_back({"VALOR", valor});
                }
            }
            else {
                resultado.tokens.push_back({"VALOR", palabra});
            }
        }
    }

    if (primerPalabra) {
        resultado.exito = false;
        resultado.error = "No se encontro comando";
    }

    return resultado;
}
