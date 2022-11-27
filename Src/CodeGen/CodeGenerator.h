//
// Created by luoli on 2022/11/13.
//

#ifndef COOLCOMPILER_CODEGENERATOR_H
#define COOLCOMPILER_CODEGENERATOR_H

#include <llvm/IR/IRBuilder.h>
#include "../Parser/AST/Program.h"
#include "Bitness.h"
#include "CodeMap.h"
#include "CStd.h"

namespace CoolCompiler {

    class CodeGenerator {
    private:
        llvm::DataLayout dataLayout;
        llvm::Module* module = nullptr;
        llvm::LLVMContext* context = nullptr;
        llvm::IRBuilder<>* builder = nullptr;
        CodeMap* codeMap;
        CStd* cStd;
        Program* program = nullptr;
        SemanticAnalyzer* analyzer = nullptr;
        Bitness getBitness();
    public:
        explicit CodeGenerator(Program* program);
        void generate(bool gcVerbose);

        llvm::Value* createAllocAndConstruct(const std::string& type);
        llvm::Value* getBoxedBasic(const std::string& type,
                                   llvm::Value* basicValue);
        llvm::Value* getLLVMDefaultValue(const std::string& type);
        llvm::Value* toLLVMIsVoid(llvm::Value* val);
        llvm::Value* addGCRoot(llvm::AllocaInst* allocInstance);
        llvm::Value* getLLVMStrEqCmp(llvm::Value* lhs_value, llvm::Value* rhs_value);
        llvm::Value* covertValue(llvm::Value* from,
                                 const std::string& currentType,
                                 const std::string& toType);
        [[nodiscard]] SemanticAnalyzer* getAnalyzer() const;
        [[nodiscard]] llvm::LLVMContext* getContext() const;
        [[nodiscard]] llvm::Module* getModule() const;
        [[nodiscard]] llvm::IRBuilder<>* getBuilder() const;
        [[nodiscard]] CodeMap* getCodeMap() const;
        [[nodiscard]] CStd* getCStd() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CODEGENERATOR_H
