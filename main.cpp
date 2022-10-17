#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/AST/AST.h"
#include "Parser/AST/Class.h"
#include "Parser/AST/Expression/Let.h"

int main() {
    CoolCompiler::Lexer l = CoolCompiler::Lexer("examples/factorial.cl");
    l.doScan();

    CoolCompiler::Parser p = CoolCompiler::Parser(l.getTokens());
    p.parse();

    auto pt = p.getParseTree();
    pt->print(0);

    return 0;
}
