//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ATMETHODACCESS_H
#define COOLCOMPILER_ATMETHODACCESS_H

#include <vector>
#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class AtMethodAccess : public Expression {
    private:
        Expression* instance;
        std::string type;
        std::string method;
        std::vector<Expression*> arguments;
    public:
        explicit AtMethodAccess(Expression* instance, const std::string &type,
                       const std::string &method, const std::vector<Expression*> &arguments);
        [[nodiscard]] Expression* getInstance() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<Expression*> getArguments() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "AT_ACCESS <" << type << "> ["  << method << "](" << std::endl;

            for(Expression *expr : arguments)
                expr->print(depth + 1);

            printTab(depth);
            std::cout << ")" << std::endl;

            printTab(depth + 1);
            std::cout << "INSTANCE:" << std::endl;
            instance->print(depth + 2);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_ATMETHODACCESS_H
