//
// Created by luoli on 2022/9/18.
//

#include "CaseAction.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    CaseAction::CaseAction(const std::string &name, const std::string &type, Expression *expression) : Expression("case_action") {
        this->name = name;
        this->type = type;
        this->expression = expression;
    }

    std::string CaseAction::getName() const {
        return name;
    }

    std::string CaseAction::getType() const {
        return type;
    }

    Expression* CaseAction::getExpression() const {
        return expression;
    }

    std::string CaseAction::typeCheck(SemanticAnalyzer *analyzer) {
        if(type == "Self")
            analyzer->fail("'self' cannot be bound in a 'branch' expression.");

        auto* objectsTable = analyzer->getObjectsTable();

        objectsTable->enter();
        objectsTable->add(name, new std::string(type));

        std::string caseActionExprType = expression->typeCheck(analyzer);

        objectsTable->exit();

        return caseActionExprType;
    }
} // CoolCompiler