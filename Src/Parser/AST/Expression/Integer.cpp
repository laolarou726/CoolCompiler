//
// Created by luoli on 2022/9/18.
//

#include <llvm/IR/Constants.h>
#include "Integer.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Integer::Integer(int value) : Expression("integer") {
        this->value = value;
    }

    int Integer::getValue() const {
        return value;
    }

    std::string Integer::typeCheck(SemanticAnalyzer *analyzer) {
        return "Int";
    }

    llvm::Value *Integer::visit(CoolCompiler::CodeGenerator *generator) {
        return llvm::ConstantInt::get(*generator->getContext(), llvm::APInt(1, value, false));
    }
} // CoolCompiler