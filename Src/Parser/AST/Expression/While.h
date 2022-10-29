//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_WHILE_H
#define COOLCOMPILER_WHILE_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class While : public Expression {
    private:
        Expression* condition;
        Expression* body;
    public:
        While(Expression* condition, Expression* body);
        [[nodiscard]] Expression* getCondition() const;
        [[nodiscard]] Expression* getBody() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            std::string conditionType = condition->typeCheck(analyzer);
            body->typeCheck(analyzer);

            if(conditionType != "Bool"){
                std::string message = fmt::format("{}: Expected the condition of while to be of type <Bool> but got the predicate of type <{}> instead.",
                                                  "While", conditionType);
                analyzer->fail(message);
            }

            return "Object";
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "WHILE [" << std::endl;

            printTab(depth + 1);
            std::cout << "WHEN [" << std::endl;

            condition->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "DO [" << std::endl;

            body->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_WHILE_H
