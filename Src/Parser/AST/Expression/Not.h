//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_NOT_H
#define COOLCOMPILER_NOT_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class Not : public Expression {
    private:
        Expression* expression;
    public:
        explicit Not(Expression *expression);
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "NOT [" << std::endl;

            expression->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_NOT_H
