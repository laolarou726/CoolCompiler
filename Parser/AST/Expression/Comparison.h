//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_COMPARISON_H
#define COOLCOMPILER_COMPARISON_H

#include "Expression.h"

namespace CoolCompiler {

    class Comparison : Expression {
    private:
        std::string operation;
        Expression expressionLeft;
        Expression expressionRight;
    public:
        Comparison(const std::string &operation, const Expression &expressionLeft, const Expression &expressionRight);
        [[nodiscard]] std::string getOperation() const;
        [[nodiscard]] Expression getExpressionLeft() const;
        [[nodiscard]] Expression getExpressionRight() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_COMPARISON_H
