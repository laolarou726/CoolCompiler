//
// Created by luoli on 2022/9/18.
//

#include "Expression.h"

namespace CoolCompiler {
    Expression::Expression(const std::string &identifier) : AST(identifier) {

    }

    Expression::Expression() : AST("expression") {

    }

    void Expression::setExpressionType(const std::string &type) {
        this->evalExprType = type;
    }

    std::string Expression::getExpressionType() const {
        return this->evalExprType;
    }
} // CoolCompiler