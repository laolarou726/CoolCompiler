#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/AST/Expression/Let.h"

int main() {
    CoolCompiler::Lexer l = CoolCompiler::Lexer("examples/factorial.cl");
    l.doScan();

    CoolCompiler::Parser p = CoolCompiler::Parser(l.getTokens());
    p.parse();

    auto pt = p.getParseTree();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
