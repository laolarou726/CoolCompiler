//
// Created by luoli on 2022/9/18.
//

#include "If.h"

namespace CoolCompiler {
    If::If(Expression* condition, Expression* conditionTrue, Expression* conditionFalse) : Expression("if") {
        this->condition = condition;
        this->conditionTrue = conditionTrue;
        this->conditionFalse = conditionFalse;
    }

    Expression* If::getCondition() const {
        return condition;
    }

    Expression* If::getConditionTrue() const {
        return conditionTrue;
    }

    Expression* If::getConditionFalse() const {
        return conditionFalse;
    }
} // CoolCompiler