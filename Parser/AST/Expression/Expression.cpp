//
// Created by luoli on 2022/9/18.
//

#include "Expression.h"

namespace CoolCompiler {
    Expression::Expression(const std::string &identifier) : AST(identifier) {

    }

    Expression::Expression() : AST("expression") {

    }
} // CoolCompiler