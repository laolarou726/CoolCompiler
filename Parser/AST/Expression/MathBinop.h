//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_MATHBINOP_H
#define COOLCOMPILER_MATHBINOP_H

#include "Expression.h"

namespace CoolCompiler {

    class MathBinop : Expression {
    private:
        std::string operation;
        Expression expressionLeft;
        Expression expressionRight;
    public:
        MathBinop(const std::string &operation, const Expression &expressionLeft, const Expression &expressionRight);
        [[nodiscard]] std::string getOperation() const;
        [[nodiscard]] Expression getExpressionLeft() const;
        [[nodiscard]] Expression getExpressionRight() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_MATHBINOP_H
