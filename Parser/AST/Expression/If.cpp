//
// Created by luoli on 2022/9/18.
//

#include "If.h"

namespace CoolCompiler {
    If::If(const Expression &condition, const AST &conditionTrue, const AST &conditionFalse) : Expression("if") {
        this->condition = condition;
        this->conditionTrue = conditionTrue;
        this->conditionFalse = conditionFalse;
    }

    Expression If::getCondition() const {
        return condition;
    }

    AST If::getConditionTrue() const {
        return conditionTrue;
    }

    AST If::getConditionFalse() const {
        return conditionFalse;
    }
} // CoolCompiler