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


} // CoolCompiler