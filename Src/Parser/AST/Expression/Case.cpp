//
// Created by luoli on 2022/9/18.
//

#include "Case.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Case::Case(Expression *expression, const std::vector<CaseAction*> &actions) : Expression("case") {
        this->expression = expression;
        this->actions = actions;
    }

    Expression* Case::getExpression() const {
        return expression;
    }

    std::vector<CaseAction*> Case::getActions() const {
        return actions;
    }

    std::string Case::typeCheck(SemanticAnalyzer *analyzer) {
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

    llvm::Value *Case::visit(CoolCompiler::CodeGenerator *generator) {

    }
} // CoolCompiler