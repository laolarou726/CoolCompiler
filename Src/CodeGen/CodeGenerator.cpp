//
// Created by luoli on 2022/11/13.
//

#include <llvm/Support/Host.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include "CodeGenerator.h"
#include "fmt/format.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    CodeGenerator::CodeGenerator(Program *program) {
        this->program = program;
    }

    Bitness CodeGenerator::getBitness() {
        const auto target_triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
        if (target_triple.isArch32Bit()) return Bitness::x32;
        if (target_triple.isArch64Bit()) return Bitness::x64;

        std::string message = fmt::format("Target triple: {} is not supported because it is not 32bit or 64bit.",
                                          target_triple.normalize());

        std::cerr << message << std::endl;

        abort();
    }

    void CodeGenerator::generate(bool gcVerbose) {
        // Initialize the target registry etc.
        LLVMInitializeX86TargetInfo();
        LLVMInitializeX86Target();
        LLVMInitializeX86TargetMC();
        LLVMInitializeX86AsmParser();
        LLVMInitializeX86AsmPrinter();
    }

    llvm::LLVMContext *CodeGenerator::getContext() const {
        return context;
    }

    llvm::Module *CodeGenerator::getModule() const {
        return module;
    }

    llvm::IRBuilder<> *CodeGenerator::getBuilder() const {
        return builder;
    }

    CStd *CodeGenerator::getCStd() const {
        return cStd;
    }

    CodeMap *CodeGenerator::getCodeMap() const {
        return codeMap;
    }

    llvm::Value *CodeGenerator::toLLVMIsVoid(llvm::Value *val) {
        return builder->CreateICmpEQ(builder->CreatePtrToInt(val, builder->getInt32Ty()),
                                     llvm::ConstantInt::get(*context, llvm::APInt(32, 0, true)));
    }

    llvm::Value *CodeGenerator::addGCRoot(llvm::AllocaInst *allocInstance) {
        llvm::Value* root = builder->CreateBitCast(
                allocInstance, codeMap->toLLVMClass("Object")->getPointerTo()->getPointerTo());
        builder->CreateCall(cStd->GCADDROOT_FUNC(), {root});

        return root;
    }

    llvm::Value *CodeGenerator::getLLVMStrEqCmp(llvm::Value* lhs_value, llvm::Value* rhs_value) {
        llvm::Value* strcmp_result =
                builder->CreateCall(cStd->STRCMP_FUNC(), {lhs_value, rhs_value});

        return builder->CreateICmpEQ(strcmp_result, codeMap->toLLVMInt32(0));
    }

    llvm::Value *CodeGenerator::getLLVMDefaultValue(const std::string &type) {
        if (type == "Int") {
            return codeMap->toLLVMInt32(0);
        }

        if (type == "String") {
            llvm::Value* malloc_val = builder->CreateCall(cStd->GCMALLOCSTRING_FUNC(), {codeMap->toLLVMInt32(1)});
            llvm::Value* str_global = builder->CreateGlobalStringPtr("");

            builder->CreateCall(cStd->STRCPY_FUNC(), {malloc_val, str_global});

            return malloc_val;
        }

        if (type == "Bool") {
            return llvm::ConstantInt::get(*context, llvm::APInt(1, 0, false));
        }

        return llvm::ConstantPointerNull::get(codeMap->toLLVMClass(type)->getPointerTo());
    }

    llvm::Value *CodeGenerator::covertValue(llvm::Value* from,
                                            const std::string& currentType,
                                            const std::string& toType) {
        if (SemanticAnalyzer::isPrimitive(currentType) && toType == "Object") {
            return CreateBoxedBasic(currentType, from);
        }

        if (SemanticAnalyzer::isPrimitive(currentType) && toType != "Object" && toType != currentType) {
            // invalid conversion, this should only occur in a case branch that's not
            // taken, so we can return anything, choose to just return default val
            return getLLVMDefaultValue(toType);
        }

        if (SemanticAnalyzer::isPrimitive(toType) && currentType == "Object") {
            return UnboxValue(toType, from);
        }

        if (currentType != toType) {
            return builder->CreateBitCast(from, codeMap->LLVM_BASIC_OR_CLASS_PTR_TYPE(toType));
        }

        return from;
    }

    llvm::Value *CodeGenerator::getBoxedBasic(const std::string &type, llvm::Value *basicValue) {
        llvm::Value* boxed_val;

        if (type == "String") {
            llvm::AllocaInst* allocaInst =
                    builder->CreateAlloca(codeMap->toBasicType("String"));
            builder->CreateStore(basicValue, allocaInst);
            builder->CreateCall(cStd->GCADDSTRINGROOT_FUNC(), {allocaInst});

            boxed_val = createAllocAndConstruct("Object");

            builder->CreateCall(cStd->GCREMOVESTRINGROOT_FUNC(), {allocaInst});
        } else {
            boxed_val = createAllocAndConstruct("Object");
        }

        llvm::Value* typeNamePtrPtr = builder->CreateStructGEP(
                codeMap->toLLVMClass("Object"), boxed_val, CodeMap::OBJ_TYPENAME_INDEX);

        builder->CreateStore(builder->CreateGlobalStringPtr(type), typeNamePtrPtr);

        llvm::Value* boxedDataPtr =
                builder->CreateStructGEP(codeMap->toLLVMClass("Object"), boxed_val,
                                         CodeMap::OBJ_BOXED_DATA_INDEX);

        llvm::Value* boxedI8PtrData = basicValue;

        if (type == "Int" || type == "Bool") {
            boxedI8PtrData = builder->CreateIntToPtr(basicValue, builder->getInt8PtrTy());
        }

        builder->CreateStore(boxedI8PtrData, boxedDataPtr);

        return boxed_val;
    }

    llvm::Value *CodeGenerator::createAllocAndConstruct(const std::string &type) {
        llvm::Type* llvmType = codeMap->toLLVMClass(type);

        const auto new_size = dataLayout.getTypeAllocSize(llvmType);
        llvm::Value* mallocLengthValue =
                llvm::ConstantInt::get(*context, llvm::APInt(32, new_size, true));

        llvm::Value* mallocValue = builder->CreateCall(
                cStd->GCMALLOC_FUNC(),
                {mallocLengthValue, builder->CreateGlobalStringPtr(type)});

        llvm::Value* new_val = builder->CreateBitCast(
                mallocValue, codeMap->toLLVMClass(type)->getPointerTo());

        std::vector<llvm::Value*> args;
        args.push_back(new_val);
        builder->CreateCall(codeMap->getConstructor(type), args);

        return new_val;
    }

    SemanticAnalyzer *CodeGenerator::getAnalyzer() const {
        return analyzer;
    }


} // CoolCompiler