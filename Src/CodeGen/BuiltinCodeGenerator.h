//
// Created by luoli on 2022/11/27.
//

#ifndef COOLCOMPILER_BUILTINCODEGENERATOR_H
#define COOLCOMPILER_BUILTINCODEGENERATOR_H

#include <llvm/IR/IRBuilder.h>
#include "CStd.h"

namespace CoolCompiler {

    class BuiltinCodeGenerator {
    private:
        llvm::LLVMContext* context;
        llvm::IRBuilder<>* builder;
        CodeMap* codeMap;
        CStd* cStd;

        void mallocStringConst(const std::string& str) const;
        void generateAbort() const;
        void generateCopy() const;
        void generateTypeName() const;
        void generateBoolTypeName() const;
        void generateStringTypeName() const;
        void generateIntTypeName() const;
        void generateBoolCopy() const;
        void generateStringCopy() const;
        void generateIntCopy() const;
    public:
        BuiltinCodeGenerator(llvm::LLVMContext* context, llvm::IRBuilder<>* builder, CodeMap* codeMap,
                             CStd* cStd);
        void generateAllFunctionBodies() const;
        void generateExitWithMessage(const std::string& msg, std::vector<llvm::Value*> printfArgs) const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_BUILTINCODEGENERATOR_H
