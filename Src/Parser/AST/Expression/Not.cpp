//
// Created by luoli on 2022/9/18.
//

#include "Not.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Not::Not(Expression *expression) : Expression("not") {
        this->expression = expression;
    }

    Expression* Not::getExpression() const {
        return expression;
    }

    std::string Not::typeCheck(SemanticAnalyzer *analyzer) {
        std::string exprType = expression->typeCheck(analyzer);

        if(exprType != "Bool"){
            std::string message = fmt::format("{}: Expected <Bool> but get <{}>.", "Not", exprType);
            analyzer->fail(message);

            return "Object";
        }

        return "Bool";
    }

    llvm::Value *Not::visit(CoolCompiler::CodeGenerator *generator) {
        llvm::Value* exprVal = expression->visit(generator);

        return generator->getBuilder()->CreateNot(exprVal);
    }
} // CoolCompiler