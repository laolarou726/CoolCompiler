//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_MATHBINOP_H
#define COOLCOMPILER_MATHBINOP_H

#include "Expression.h"
#include "../../../Lexer/Token.h"

namespace CoolCompiler {

    class MathBinop : public Expression {
    private:
        TokenType operation;
        Expression* expressionLeft;
        Expression* expressionRight;
    public:
        MathBinop(TokenType operation, Expression *expressionLeft, Expression *expressionRight);
        [[nodiscard]] TokenType getOperation() const;
        [[nodiscard]] Expression* getExpressionLeft() const;
        [[nodiscard]] Expression* getExpressionRight() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_MATHBINOP_H
