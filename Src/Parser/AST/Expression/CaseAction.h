//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CASEACTION_H
#define COOLCOMPILER_CASEACTION_H

#include "Expression.h"

namespace CoolCompiler {

    class CaseAction : Expression {
    private:
        std::string name;
        std::string type;
        Expression* expression;
    public:
        CaseAction(const std::string &name, const std::string &type, Expression *expression);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Expression* getExpression() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            if(type == "Self")
                analyzer->fail("'self' cannot be bound in a 'branch' expression.");

            auto* objectsTable = analyzer->getObjectsTable();

            objectsTable->enter();
            objectsTable->add(name, &type);

            std::string caseActionExprType = expression->typeCheck(analyzer);

            objectsTable->exit();

            return caseActionExprType;
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "CASE_ACTION [" << name << "]<"  << type << ">" << std::endl;

            expression->print(depth + 1);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASEACTION_H
