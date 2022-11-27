//
// Created by luoli on 2022/9/18.
//

#include "Let.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Let::Let(const std::vector<InnerLet*> &parameters, Expression* expression) : Expression("let") {
        this->parameters = parameters;
        this->expression = expression;
    }

    std::vector<InnerLet*> Let::getParameters() const {
        return parameters;
    }

    Expression* Let::getExpression() const {
        return expression;
    }

    std::string Let::typeCheck(SemanticAnalyzer *analyzer) {
        auto* objectsTable = analyzer->getObjectsTable();

        objectsTable->enter();

        for(auto* innerLet : parameters){
            std::string innerLetType = innerLet->typeCheck(analyzer);
            objectsTable->add(innerLet->getName(), new std::string(innerLetType));
        }

        std::string resultType = expression->typeCheck(analyzer);

        objectsTable->exit();

        return resultType;
    }

    llvm::Value *Let::visit(CoolCompiler::CodeGenerator *generator) {

    }
} // CoolCompiler