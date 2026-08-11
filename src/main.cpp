#include "lexer.h"
#include "parser.h"
#include <iostream>

int main() {
    Lexer lexer;
    Parser parser;
    std::string entrada;

    std::cout << "Analizador EXT2" << std::endl;
    std::cout << "Escribe un comando o 'salir' para terminar" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, entrada);

        if (entrada == "salir") break;

        ResultadoLexico resLexico = lexer.analizar(entrada);

        if (!resLexico.exito) {
            std::cout << "ERROR LEXICO: " << resLexico.error << std::endl;
            continue;
        }

        std::cout << "TOKENS: ";
        for (const auto& token : resLexico.tokens) {
            std::cout << "[" << token.tipo << ":" << token.valor << "] ";
        }
        std::cout << std::endl;

        ResultadoSintactico resSintactico = parser.analizar(resLexico.tokens);

        if (!resSintactico.valido) {
            std::cout << "ERRORES:" << std::endl;
            for (const auto& error : resSintactico.errores) {
                std::cout << "  - " << error << std::endl;
            }
        } else {
            std::cout << "COMANDO VALIDO" << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
