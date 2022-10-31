//
// Created by luoli on 2022/9/18.
//

#include "InnerLet.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    InnerLet::InnerLet(const std::string &name, const std::string &type) : Expression("inner_let") {
        this->name = name;
        this->type = type;
    }

    InnerLet::InnerLet(const std::string &name, const std::string &type, Expression* expression) : Expression("inner_let") {
        this->name = name;
        this->type = type;
        this->expression = expression;
    }

    std::string InnerLet::getName() const {
        return name;
    }

    std::string InnerLet::getType() const {
        return type;
    }

    Expression* InnerLet::getExpression() const {
        return expression;
    }

    std::string InnerLet::typeCheck(SemanticAnalyzer *analyzer) {
        if(name == "Self")
            analyzer->fail("'self' cannot be bound in a 'let' expression.");

        std::string initType = "_no_type";
        if(expression != nullptr)
            initType = expression->typeCheck(analyzer);

        if(type != "SELF_TYPE" && !analyzer->isTypeDefined(type)){
            std::string message = fmt::format("Type <{}> of let-bound identifier [{}] is undefined.",
                                              type, name);
            analyzer->fail(message);
        }
        else if(initType != "_no_type" && !analyzer->isSubtype(initType, type)){
            std::string message = fmt::format("Inferred type <{}> in initialization of [{}] is not compatible with identifier's declared type <{}>.",
                                              initType, name, type);
            analyzer->fail(message);
        }

        return initType == "_no_type" ? type : initType;
    }
} // CoolCompiler