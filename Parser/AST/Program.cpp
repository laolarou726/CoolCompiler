//
// Created by luoli on 2022/9/18.
//

#include "Program.h"

namespace CoolCompiler {
    Program::Program(const std::vector<AST> &classes) : AST("program") {
        this->classes = classes;
    }

    std::vector<AST> Program::getClasses() const {
        return classes;
    }
} // CoolCompiler