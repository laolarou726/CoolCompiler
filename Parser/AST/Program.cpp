//
// Created by luoli on 2022/9/18.
//

#include "Program.h"

namespace CoolCompiler {
    Program::Program() : AST("program") {
        this->classes = new std::vector<AST*>();
    }

    std::vector<AST*>* Program::getClasses() const {
        return classes;
    }
} // CoolCompiler