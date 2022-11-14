//
// Created by luoli on 2022/9/18.
//

#include "Boolean.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"
#include <llvm/IR/Constants.h>

namespace CoolCompiler {
    Boolean::Boolean(bool value) : Expression("boolean") {
        this->value = value;
    }

    bool Boolean::getValue() const {
        return value;
    }

    std::string Boolean::typeCheck(SemanticAnalyzer *analyzer) {
        return "Bool";
    }

    llvm::Value *Boolean::visit(CoolCompiler::CodeGenerator *generator) {
        auto* context = generator->getContext();

        return this->value ?
               llvm::ConstantInt::get(*context, llvm::APInt(1, 1, false)) :
               llvm::ConstantInt::get(*context, llvm::APInt(1, 0, false));
    }
} // CoolCompiler