//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CASE_H
#define COOLCOMPILER_CASE_H

#include <vector>
#include <set>
#include "Expression.h"
#include "CaseAction.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class Case : public Expression {
    private:
        Expression* expression;
        std::vector<CaseAction*> actions;
    public:
        Case(Expression *expression, const std::vector<CaseAction*> &actions);
        [[nodiscard]] Expression* getExpression() const;
        [[nodiscard]] std::vector<CaseAction*> getActions() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            std::string exprType = expression->typeCheck(analyzer);

            std::set<std::string> caseTypeMap;
            std::string result = "Object";
            int index = 0;

            for(auto* action : actions){
                std::string actionType = action->getType();

                if(caseTypeMap.find(actionType) != caseTypeMap.end()){
                    std::string message = fmt::format("{}: Duplicate branch type <{}> in case statement.",
                                                      action->getName(), action->getType());
                    analyzer->fail(message);
                }
                else{
                    caseTypeMap.emplace(actionType);
                }

                if(index == 0){
                    result = action->typeCheck(analyzer);
                }
                else{
                    result = analyzer->leastCommonAncestorType(result, action->typeCheck(analyzer));
                }

                index++;
            }

            return result;
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "CASE [" << std::endl;

            expression->print(depth + 1);

            std::cout << "]" << std::endl;

            for(CaseAction *ca : actions)
                ca->print(depth + 1);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASE_H
