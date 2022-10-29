//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_LET_H
#define COOLCOMPILER_LET_H

#include <vector>
#include "Expression.h"
#include "InnerLet.h"

namespace CoolCompiler {

    class Let : public Expression {
    private:
        std::vector<InnerLet*> parameters;
        Expression* expression;
    public:
        Let(const std::vector<InnerLet*> &parameters, Expression* expression);
        [[nodiscard]] std::vector<InnerLet*> getParameters() const;
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            auto* objectsTable = analyzer->getObjectsTable();

            objectsTable->enter();
            if()
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "LET" << std::endl;

            for(InnerLet *il : parameters)
                il->print(depth + 1);

            printTab(depth);
            std::cout << "IN" << std::endl;

            expression->print(depth + 1);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_LET_H
