//
// Created by luoli on 2022/9/18.
//

#include "Not.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Not::Not(Expression *expression) : Expression("not") {
        this->expression = expression;
    }

    Expression* Not::getExpression() const {
        return expression;
    }

    std::string Not::typeCheck(SemanticAnalyzer *analyzer) {
        std::string exprType = expression->typeCheck(analyzer);

        if(exprType != "Bool"){
            std::string message = fmt::format("{}: Expected <Bool> but get <{}>.", "Not", exprType);
            analyzer->fail(message);

            return "Object";
        }

        return "Bool";
    }
} // CoolCompiler