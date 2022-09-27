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

    std::vector<CoolCompiler::AST*> tt;
    auto* xx = new CoolCompiler::Class("123", tt, "123");
    tt.emplace_back(xx);



    std::cout << ((CoolCompiler::Class*)tt[0])->getName() << "Hello, World!" << std::endl;
    return 0;
}
