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

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            std::string name = id->getName();

            if(name == "Self"){
                std::string message = fmt::format("{}: Cannot assign to 'self'.", "Assignment");
                analyzer->fail(message);
            }

            std::string* idType = analyzer->getObjectsTable()->lookup(name);

            if(idType == nullptr){
                std::string message = fmt::format("{}: Tried to assign undeclared identifier [{}].",
                                                  "Assignment", name);
                analyzer->fail(message);

                return "Object";
            }

            std::string exprType = expression->typeCheck(analyzer);

            bool isAssignValid = analyzer->isSubtype(exprType, *idType);

            if(!isAssignValid){
                std::string message = fmt::format("{}: The identifier [{}] has been declared as <{}> but assigned with incompatible type <{}>.",
                                                  "Assignment", name, *idType, exprType);
                analyzer->fail(message);

                return "Object";
            }

            return exprType;
        }

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
