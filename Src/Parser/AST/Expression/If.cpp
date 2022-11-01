//
// Created by luoli on 2022/9/18.
//

#include "If.h"
#include "../../../Semantic/SemanticAnalyzer.h"

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

    std::string If::typeCheck(SemanticAnalyzer *analyzer) {
        std::string conditionType = condition->typeCheck(analyzer);
        std::string trueType = conditionTrue->typeCheck(analyzer);
        std::string falseType = conditionFalse->typeCheck(analyzer);

        trueType = trueType == "SELF_TYPE" ? analyzer->getCurrentClassName() : trueType;
        falseType = falseType == "SELF_TYPE" ? analyzer->getCurrentClassName() : falseType;

        if(conditionType != "Bool"){
            std::string message = fmt::format("{}: Expected the condition of if to be of type <Bool> but got the predicate of type <{}> instead.",
                                              "If", conditionType);
            analyzer->fail(message);
        }

        std::string result = analyzer->leastCommonAncestorType(trueType, falseType);
        return result;
    }
} // CoolCompiler