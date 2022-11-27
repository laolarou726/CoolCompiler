//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_COMPARISON_H
#define COOLCOMPILER_COMPARISON_H

#include <llvm/IR/IRBuilder.h>
#include "Expression.h"
#include "fmt/format.h"
#include "../../../Lexer/Token.h"
#include "../../../CodeGen/CStd.h"

namespace CoolCompiler {

    class Comparison : public Expression {
    private:
        std::string token;
        TokenType operation;
        Expression *expressionLeft;
        Expression *expressionRight;
    public:
        Comparison(const std::string &token, TokenType operation, Expression *expressionLeft, Expression *expressionRight);
        [[nodiscard]] TokenType getOperation() const;
        [[nodiscard]] Expression* getExpressionLeft() const;
        [[nodiscard]] Expression* getExpressionRight() const;

        std::string eqCheck(SemanticAnalyzer* analyzer);
        std::string boolOpCheck(SemanticAnalyzer* analyzer, const std::string &symbol);
        llvm::Value* getLLVMEq(SemanticAnalyzer* analyzer,
                               CoolCompiler::CodeGenerator *generator,
                               llvm::Value* lhs,
                               llvm::Value* rhs);

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "COMPARE [" << std::endl;

            expressionLeft->print(depth + 1);

            std::cout << operation << std::endl;

            expressionRight->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_COMPARISON_H
