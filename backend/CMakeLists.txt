cmake_minimum_required(VERSION 3.10)
project(analizador_ext2)

set(CMAKE_CXX_STANDARD 17)

add_executable(analizador
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

target_include_directories(analizador PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
