//
// Created by luoli on 2022/9/18.
//

#include "Case.h"

namespace CoolCompiler {
    Case::Case(Expression *expression, const std::vector<CaseAction*> &actions) : Expression("case") {
        this->expression = expression;
        this->actions = actions;
    }

    Expression* Case::getExpression() const {
        return expression;
    }

    std::vector<CaseAction*> Case::getActions() const {
        return actions;
    }
} // CoolCompiler