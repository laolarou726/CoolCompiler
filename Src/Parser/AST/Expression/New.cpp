//
// Created by luoli on 2022/9/18.
//

#include "New.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    New::New(const std::string &type) : Expression("new") {
        this->type = type;
    }

    std::string New::getType() const {
        return type;
    }

    std::string New::typeCheck(SemanticAnalyzer *analyzer) {
        if(type != "SELF_TYPE" && !analyzer->isTypeDefined(type)){
            std::string message = fmt::format("{}: Tried to instantiate an object of undefined type: {}.",
                                              "New", type);
            analyzer->fail(message);

            return "Object";
        }

        return type;
    }

    llvm::Value *New::visit(CoolCompiler::CodeGenerator *generator) {
        auto* codeMap = generator->getCodeMap();
        auto* builder = generator->getBuilder();

        if (codeMap->toBasicType(type) != nullptr) {
            return generator->getLLVMDefaultValue(type);
        }

        if (type == "SELF_TYPE") {
            llvm::Value* currentClassValue = codeMap->getCurrentLLVMFunction()->args().begin();
            llvm::Value* currentClassValueAsObj =
                    generator->covertValue(currentClassValue, codeMap->getCurrentClass()->getName(), "Object");
            llvm::Value* copied = builder->CreateCall(
                    codeMap->getLLVMFunction("Object", "copy"), {currentClassValueAsObj});

            llvm::Value* constructor_func_ptr = builder->CreateStructGEP(
                    nullptr, copied, CodeMap::OBJ_CONSTRUCTOR_INDEX);

            llvm::Function* constructorFunction = builder->CreateLoad(codeMap->toLLVMClass(type), constructor_func_ptr)->getFunction();
            builder->CreateCall(constructorFunction, {copied});

            return copied;
        }

        return generator->createAllocAndConstruct(type);
    }
} // CoolCompiler