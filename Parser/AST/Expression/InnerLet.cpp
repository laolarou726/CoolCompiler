//
// Created by luoli on 2022/9/18.
//

#include "InnerLet.h"

namespace CoolCompiler {
    InnerLet::InnerLet(const std::string &name, const std::string &type) : Expression("inner_let") {
        this->name = name;
        this->type = type;
    }

    InnerLet::InnerLet(const std::string &name, const std::string &type, const Expression &expression) : Expression("inner_let") {
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

    Expression InnerLet::getExpression() const {
        return expression;
    }
} // CoolCompiler