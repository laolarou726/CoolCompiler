//
// Created by luoli on 2022/9/18.
//

#include "IsVoid.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    IsVoid::IsVoid(Expression *expression) : Expression("is_void") {
        this->expression = expression;
    }

    Expression* IsVoid::getExpression() const {
        return expression;
    }

    std::string IsVoid::typeCheck(SemanticAnalyzer *analyzer) {
        expression->typeCheck(analyzer);
        return "Bool";
    }
} // CoolCompiler