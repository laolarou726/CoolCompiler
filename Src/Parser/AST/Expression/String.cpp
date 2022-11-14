//
// Created by luoli on 2022/9/18.
//

#include "String.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    String::String(const std::string &value) : Expression("string") {
        this->value = value;
    }

    std::string String::getValue() const {
        return value;
    }

    std::string String::typeCheck(SemanticAnalyzer *analyzer) {
        return "String";
    }

    llvm::Value *String::visit(CoolCompiler::CodeGenerator *generator) {
        auto* builder = generator->getBuilder();

        llvm::Value* malloc_val =
                builder->CreateCall(generator->getCStd()->GCMALLOCSTRING_FUNC(),
                                    {generator->getCodeMap()->toLLVMInt32(value.length() + 1)});
        llvm::Value* str_global = builder->CreateGlobalStringPtr(value);
        builder->CreateCall(generator->getCStd()->STRCPY_FUNC(), {malloc_val, str_global});

        return malloc_val;
    }
} // CoolCompiler