//
// Created by luoli on 2022/9/18.
//

#include "Case.h"

namespace CoolCompiler {
    Case::Case(const Expression &expression, const std::vector<AST> &actions) : Expression("case") {
        this->expression = expression;
        this->actions = actions;
    }

    Expression Case::getExpression() const {
        return expression;
    }

    std::vector<AST> Case::getActions() const {
        return actions;
    }
} // CoolCompiler