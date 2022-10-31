//
// Created by luoli on 2022/9/18.
//

#include "Program.h"

namespace CoolCompiler {
    Program::Program() : AST("program") {
        this->classes = new std::vector<Class*>();
    }

    std::vector<Class*>* Program::getClasses() const {
        return classes;
    }
} // CoolCompiler