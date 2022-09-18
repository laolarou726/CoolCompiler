#include <iostream>
#include "Lexer/Lexer.h"

int main() {
    CoolCompiler::Lexer l = CoolCompiler::Lexer("examples/classes.cl");
    l.doScan();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
