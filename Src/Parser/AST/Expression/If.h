//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_IF_H
#define COOLCOMPILER_IF_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class If : public Expression {
    private:
        Expression* condition;
        Expression* conditionTrue;
        Expression* conditionFalse;
    public:
        If(Expression* condition, Expression* conditionTrue, Expression* conditionFalse);
        [[nodiscard]] Expression* getCondition() const;
        [[nodiscard]] Expression* getConditionTrue() const;
        [[nodiscard]] Expression* getConditionFalse() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            std::string conditionType = condition->typeCheck(analyzer);
            std::string trueType = conditionTrue->typeCheck(analyzer);
            std::string falseType = conditionFalse->typeCheck(analyzer);

            if(conditionType != "Bool"){
                std::string message = fmt::format("{}: Expected the condition of if to be of type <Bool> but got the predicate of type <{}> instead.",
                                                  "If", conditionType);
                analyzer->fail(message);
            }

            std::string result = analyzer->leastCommonAncestorType(trueType, falseType);
            return result;
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "IF [" << std::endl;

            printTab(depth + 1);
            std::cout << "CONDITION [" << std::endl;

            condition->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "TRUE [" << std::endl;

            conditionTrue->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "FALSE [" << std::endl;

            conditionFalse->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_IF_H
