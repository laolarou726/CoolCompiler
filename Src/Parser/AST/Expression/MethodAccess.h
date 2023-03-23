//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_METHODACCESS_H
#define COOLCOMPILER_METHODACCESS_H

#include <vector>
#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class MethodAccess : public Expression {
    private:
        Expression* instance;
        std::string method;
        std::vector<Expression*> arguments;
    public:
        explicit MethodAccess(Expression* instance, const std::string &method, const std::vector<Expression*> &arguments);
        [[nodiscard]] Expression* getInstance() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<Expression*> getArguments() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "METHOD_ACCESS [" << std::endl;

            instance->print(depth + 1);
            printTab(depth + 1);
            std::cout << "=> " << method << "(" << std::endl;

            for(Expression *expr : arguments)
                expr->print(depth + 2);

            printTab(depth + 1);
            std::cout << ")" << std::endl;

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_METHODACCESS_H
