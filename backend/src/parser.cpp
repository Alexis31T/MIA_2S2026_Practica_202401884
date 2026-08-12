#include "parser.h"
#include <algorithm>

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

    // Validar segun comando
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

ResultadoSintactico Parser::validarMKDISK(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MKDISK", params, {}};

    if (params.find("-size") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -size");
        resultado.valido = false;
    } else {
        try {
            int size = std::stoi(params.at("-size"));
            if (size <= 0) {
                resultado.errores.push_back("El tamanio debe ser positivo");
                resultado.valido = false;
            }
        } catch (...) {
            resultado.errores.push_back("-size debe ser un numero");
            resultado.valido = false;
        }
    }

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    auto unitIt = params.find("-unit");
    if (unitIt != params.end()) {
        std::string unit = unitIt->second;
        if (unit != "K" && unit != "M") {
            resultado.errores.push_back("Unidad invalida. Debe ser K o M");
            resultado.valido = false;
        }
    }

    auto fitIt = params.find("-fit");
    if (fitIt != params.end()) {
        std::string fit = fitIt->second;
        if (fit != "BF" && fit != "FF" && fit != "WF") {
            resultado.errores.push_back("Ajuste invalido. Debe ser BF, FF o WF");
            resultado.valido = false;
        }
    }

    return resultado;
}

ResultadoSintactico Parser::validarRMDISK(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "RMDISK", params, {}};

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    return resultado;
}

ResultadoSintactico Parser::validarFDISK(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "FDISK", params, {}};

    if (params.find("-size") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -size");
        resultado.valido = false;
    } else {
        try {
            int size = std::stoi(params.at("-size"));
            if (size <= 0) {
                resultado.errores.push_back("El tamanio debe ser positivo");
                resultado.valido = false;
            }
        } catch (...) {
            resultado.errores.push_back("-size debe ser un numero");
            resultado.valido = false;
        }
    }

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    if (params.find("-name") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -name");
        resultado.valido = false;
    }

    auto unitIt = params.find("-unit");
    if (unitIt != params.end()) {
        std::string unit = unitIt->second;
        if (unit != "B" && unit != "K" && unit != "M") {
            resultado.errores.push_back("Unidad invalida. Debe ser B, K o M");
            resultado.valido = false;
        }
    }

    auto typeIt = params.find("-type");
    if (typeIt != params.end()) {
        std::string type = typeIt->second;
        if (type != "P" && type != "E" && type != "L") {
            resultado.errores.push_back("Tipo invalido. Debe ser P, E o L");
            resultado.valido = false;
        }
    }

    auto fitIt = params.find("-fit");
    if (fitIt != params.end()) {
        std::string fit = fitIt->second;
        if (fit != "BF" && fit != "FF" && fit != "WF") {
            resultado.errores.push_back("Ajuste invalido. Debe ser BF, FF o WF");
            resultado.valido = false;
        }
    }

    return resultado;
}

ResultadoSintactico Parser::validarMOUNT(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MOUNT", params, {}};

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    if (params.find("-name") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -name");
        resultado.valido = false;
    }

    return resultado;
}

ResultadoSintactico Parser::validarMKFS(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MKFS", params, {}};

    if (params.find("-id") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -id");
        resultado.valido = false;
    }

    auto typeIt = params.find("-type");
    if (typeIt != params.end() && typeIt->second != "full") {
        resultado.errores.push_back("El tipo debe ser 'full'");
        resultado.valido = false;
    }

    return resultado;
}

ResultadoSintactico Parser::validarMKUSR(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MKUSR", params, {}};

    if (params.find("-user") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -user");
        resultado.valido = false;
    } else if (params.at("-user").length() > 10) {
        resultado.errores.push_back("Usuario no debe exceder 10 caracteres");
        resultado.valido = false;
    }

    if (params.find("-pass") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -pass");
        resultado.valido = false;
    } else if (params.at("-pass").length() > 10) {
        resultado.errores.push_back("Contrasenia no debe exceder 10 caracteres");
        resultado.valido = false;
    }

    if (params.find("-grp") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -grp");
        resultado.valido = false;
    } else if (params.at("-grp").length() > 10) {
        resultado.errores.push_back("Grupo no debe exceder 10 caracteres");
        resultado.valido = false;
    }

    return resultado;
}

ResultadoSintactico Parser::validarRMUSR(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "RMUSR", params, {}};

    if (params.find("-user") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -user");
        resultado.valido = false;
    }

    return resultado;
}

ResultadoSintactico Parser::validarMKFILE(const std::map<std::string, std::string>& params) {
    ResultadoSintactico resultado{true, "MKFILE", params, {}};

    if (params.find("-path") == params.end()) {
        resultado.errores.push_back("Falta parametro obligatorio: -path");
        resultado.valido = false;
    }

    auto sizeIt = params.find("-size");
    if (sizeIt != params.end()) {
        try {
            int size = std::stoi(sizeIt->second);
            if (size < 0) {
                resultado.errores.push_back("El tamanio no puede ser negativo");
                resultado.valido = false;
            }
        } catch (...) {
            resultado.errores.push_back("-size debe ser un numero");
            resultado.valido = false;
        }
    }

    return resultado;
}
