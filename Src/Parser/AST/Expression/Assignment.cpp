//
// Created by luoli on 2022/9/18.
//

#include "Assignment.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Assignment::Assignment(Id* id, Expression* expression) : Expression("assignment")  {
        this->id = id;
        this->expression = expression;
    }

    Id* Assignment::getId() const {
        return id;
    }

    Expression* Assignment::getExpression() const {
        return expression;
    }

    std::string Assignment::typeCheck(SemanticAnalyzer *analyzer) {
        std::string name = id->getName();

        if(name == "Self"){
            std::string message = fmt::format("{}: Cannot assign to 'self'.", "Assignment");
            analyzer->fail(message);
        }

        std::string* idType = analyzer->getObjectsTable()->lookup(name);

        if(idType == nullptr){
            std::string message = fmt::format("{}: Tried to assign undeclared identifier [{}].",
                                              "Assignment", name);
            analyzer->fail(message);

            return "Object";
        }

        std::string exprType = expression->typeCheck(analyzer);

        bool isAssignValid = analyzer->isSubtype(exprType, *idType);

        if(!isAssignValid){
            std::string message = fmt::format("{}: The identifier [{}] has been declared as <{}> but assigned with incompatible type <{}>.",
                                              "Assignment", name, *idType, exprType);
            analyzer->fail(message);

            return "Object";
        }

        return exprType;
    }
} // CoolCompiler