//
// Created by luoli on 2022/9/18.
//

#include "IsVoid.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include <llvm/IR/Value.h>
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    IsVoid::IsVoid(Expression *expression) : Expression("is_void") {
        this->expression = expression;
    }

    Expression* IsVoid::getExpression() const {
        return expression;
    }

    std::string IsVoid::typeCheck(SemanticAnalyzer *analyzer) {
        expression->typeCheck(analyzer);
        return "Bool";
    }

    llvm::Value *IsVoid::visit(CoolCompiler::CodeGenerator *generator) {
        llvm::Value* exprVal = expression->visit(generator);

        return generator->toLLVMIsVoid(exprVal);
    }
} // CoolCompiler