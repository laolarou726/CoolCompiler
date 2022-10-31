//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_BLOCK_H
#define COOLCOMPILER_BLOCK_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class Block : public Expression {
    private:
        std::vector<Expression*> expressions;
    public:
        explicit Block(const std::vector<Expression*> &expressions);
        [[nodiscard]] std::vector<Expression*> getExpressions() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "BLOCK [" << std::endl;

            for(Expression *expr : expressions)
                expr->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_BLOCK_H
