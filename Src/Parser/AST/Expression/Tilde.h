//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_TILDE_H
#define COOLCOMPILER_TILDE_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class Tilde : public Expression {
    private:
        Expression* expression;
    public:
        explicit Tilde(Expression *expression);
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "TILDE [" << std::endl;

            expression->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_TILDE_H
