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

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

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
