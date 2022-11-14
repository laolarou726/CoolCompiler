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
        llvm::Module* module = nullptr;
        llvm::LLVMContext* context = nullptr;
        llvm::IRBuilder<>* builder = nullptr;
        CodeMap* codeMap;
        CStd* cStd;
        Program* program = nullptr;
        Bitness getBitness();
    public:
        explicit CodeGenerator(Program* program);
        void generate(bool gcVerbose);
        [[nodiscard]] llvm::LLVMContext* getContext() const;
        [[nodiscard]] llvm::Module* getModule() const;
        [[nodiscard]] llvm::IRBuilder<>* getBuilder() const;
        [[nodiscard]] CodeMap* getCodeMap() const;
        [[nodiscard]] CStd* getCStd() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CODEGENERATOR_H
