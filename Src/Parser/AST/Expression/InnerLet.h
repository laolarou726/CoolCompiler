//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_INNERLET_H
#define COOLCOMPILER_INNERLET_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class InnerLet : Expression {
    private:
        std::string name;
        std::string type;
        Expression* expression;
    public:
        InnerLet(const std::string &name, const std::string &type);
        InnerLet(const std::string &name, const std::string &type, Expression* expression);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << name << " : <" << type << ">[ <-" << std::endl;

            expression->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_INNERLET_H
