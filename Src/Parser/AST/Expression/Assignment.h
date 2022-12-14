//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ASSIGNMENT_H
#define COOLCOMPILER_ASSIGNMENT_H

#include "Expression.h"
#include "Id.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class Assignment : public Expression {
    private:
        Id* id;
        Expression* expression;
    public:
        Assignment(Id* id, Expression* expression);
        [[nodiscard]] Id* getId() const;
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value* getAssignmentLhsPtr(CoolCompiler::CodeGenerator *generator);
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "(" << std::endl;

            id->print(depth + 1);

            printTab(depth + 1);
            std::cout << "<-" << std::endl;

            expression->print(depth + 1);

            printTab(depth);
            std::cout << ")" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_ASSIGNMENT_H
