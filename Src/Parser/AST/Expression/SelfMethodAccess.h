//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_SELFMETHODACCESS_H
#define COOLCOMPILER_SELFMETHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class SelfMethodAccess : public Expression {
    private:
        std::string method;
        std::vector<Expression*> arguments;
    public:
        explicit SelfMethodAccess(const std::string &method, const std::vector<Expression*> &arguments);
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<Expression*> getArguments() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "SELF_METHOD_ACCESS [" << std::endl;

            printTab(depth + 1);
            std::cout << method << "(" << std::endl;

            for(Expression *expr : arguments)
                expr->print(depth + 2);

            printTab(depth + 1);
            std::cout << ")" << std::endl;

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_SELFMETHODACCESS_H
