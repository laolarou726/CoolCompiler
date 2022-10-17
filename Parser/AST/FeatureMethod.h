//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_FEATUREMETHOD_H
#define COOLCOMPILER_FEATUREMETHOD_H

#include <vector>
#include "AST.h"
#include "Expression/Expression.h"
#include "Formal.h"

namespace CoolCompiler {

    class FeatureMethod : public AST {
    private:
        std::string name;
        std::string returnType;
        Expression* expression;
        std::vector<Formal*> formalArguments;
    public:
        FeatureMethod(const std::string &name, const std::string &returnType,
                      Expression* expression, const std::vector<Formal*> &formalArguments);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getReturnType() const;
        [[nodiscard]] Expression* getExpression() const;
        [[nodiscard]] std::vector<Formal*> getFormalArguments() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "METHOD <" << returnType << "> " << name << " (" << std::endl;

            for(Formal *ast : formalArguments)
                ast->print(depth + 1);

            printTab(depth);
            std::cout << ") => [" << std::endl;

            expression->print(depth + 2);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

}


#endif //COOLCOMPILER_FEATUREMETHOD_H
