#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <map>

struct ResultadoSintactico {
    bool valido;
    std::string comando;
    std::map<std::string, std::string> parametros;
    std::vector<std::string> errores;
};

class Parser {
public:
    ResultadoSintactico analizar(const std::vector<Token>& tokens);

private:
    ResultadoSintactico validarMKDISK(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarRMDISK(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarFDISK(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarMOUNT(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarMKFS(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarMKUSR(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarRMUSR(const std::map<std::string, std::string>& params);
    ResultadoSintactico validarMKFILE(const std::map<std::string, std::string>& params);
};

#endif
