//
// Created by luoli on 2022/9/18.
//

#include "AST.h"

namespace CoolCompiler {
    AST::AST(const std::string &identifier) {
        this->identifier = identifier;
    }

    std::string AST::getIdentifier() const {
        return identifier;
    }

    AST::AST() {
        this->identifier = "undefined_ast_block";
    }
} // CoolCompiler