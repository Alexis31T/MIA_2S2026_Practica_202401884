#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

struct Token {
    std::string tipo;
    std::string valor;
};

struct ResultadoLexico {
    bool exito;
    std::vector<Token> tokens;
    std::string error;
};

class Lexer {
public:
    ResultadoLexico analizar(const std::string& entrada);
};

#endif
