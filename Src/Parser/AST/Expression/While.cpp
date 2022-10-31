//
// Created by luoli on 2022/9/18.
//

#include "While.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    While::While(Expression* condition, Expression* body) : Expression("while") {
        this->condition = condition;
        this->body = body;
    }

    Expression* While::getCondition() const {
        return condition;
    }

    Expression* While::getBody() const {
        return body;
    }

    std::string While::typeCheck(SemanticAnalyzer *analyzer) {
        std::string conditionType = condition->typeCheck(analyzer);
        body->typeCheck(analyzer);

        if(conditionType != "Bool"){
            std::string message = fmt::format("{}: Expected the condition of while to be of type <Bool> but got the predicate of type <{}> instead.",
                                              "While", conditionType);
            analyzer->fail(message);
        }

        return "Object";
    }
} // CoolCompiler