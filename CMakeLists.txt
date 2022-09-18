cmake_minimum_required(VERSION 3.23)
project(CoolCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(CoolCompiler main.cpp Lexer/Token.cpp Lexer/Token.h Lexer/Error.cpp Lexer/Error.h Lexer/Lexer.cpp Lexer/Lexer.h)
