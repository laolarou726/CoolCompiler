//
// Created by luoli on 2022/11/13.
//

#include "CStd.h"
#include "CodeMap.h"

namespace CoolCompiler {
    llvm::FunctionCallee CStd::createCStdFuncDecl(const std::string &func_name, const std::string &return_type_str,
                                             const std::vector<std::string> &arg_types, bool is_var_arg) const {
        std::vector<llvm::Type*> llvm_arg_types;
        for (const auto& arg : arg_types) {
            llvm_arg_types.push_back(codeMap->toBasicType(arg));
        }

        llvm::Type* return_type;
        if (return_type_str == "Void") {
            return_type = codeMap->VOID_TYPE();
        } else {
            return_type = codeMap->toBasicType(return_type_str);
        }

        return createCStdFuncDecl(func_name, return_type, llvm_arg_types, is_var_arg);
    }

    llvm::FunctionCallee CStd::createCStdFuncDecl(const std::string &func_name, llvm::Type *return_type,
                                             const std::vector<llvm::Type *> &llvm_arg_types, bool is_var_arg) const {
        llvm::FunctionType* func_type =
                llvm::FunctionType::get(return_type, llvm_arg_types, is_var_arg);
        llvm::FunctionCallee callee = module->getOrInsertFunction(func_name, func_type);

        return callee;
    }

    llvm::FunctionCallee CStd::PRINTF_FUNC() const {
        return createCStdFuncDecl("printf", "Int", {"String"}, true);
    }

    llvm::FunctionCallee CStd::STRLEN_FUNC() const {
        return createCStdFuncDecl("strlen", "Int", {"String"});
    }

    llvm::FunctionCallee CStd::STRCPY_FUNC() const {
        return createCStdFuncDecl("strcpy", "String", {"String", "String"});
    }

    llvm::FunctionCallee CStd::STRNCPY_FUNC() const {
        return createCStdFuncDecl("strncpy", "String", {"String", "String", "Int"});
    }

    llvm::FunctionCallee CStd::STRCAT_FUNC() const {
        return createCStdFuncDecl("strcat", "String", {"String", "String"});
    }

    llvm::FunctionCallee CStd::EXIT_FUNC() const {
        return createCStdFuncDecl("exit", "Void", {"Int"});
    }

    llvm::FunctionCallee CStd::STRCMP_FUNC() const {
        return createCStdFuncDecl("exit", "Void", {"Int"});
    }

    llvm::FunctionCallee CStd::GETCHAR_FUNC() const {
        return createCStdFuncDecl("getchar", "Int", {});
    }

    llvm::FunctionCallee CStd::ATOI_FUNC() const {
        return createCStdFuncDecl("atoi", "Int", {"String"});
    }

    llvm::FunctionCallee CStd::GCMALLOC_FUNC() const {
        return createCStdFuncDecl("gc_malloc", "String", {"Int", "String"});
    }

    llvm::FunctionCallee CStd::GCCOPYOBJ_FUNC() const {
        return createCStdFuncDecl("gc_copy_obj", codeMap->VOID_TYPE(),
                                  {codeMap->toLLVMClass("Object")->getPointerTo(),
                                   codeMap->toLLVMClass("Object")->getPointerTo(),
                                   codeMap->toBasicType("Int")},
                                  false);
    }

    llvm::FunctionCallee CStd::GCMALLOCSTRING_FUNC() const {
        return createCStdFuncDecl("gc_malloc_string", "String", {"Int"});;
    }

    llvm::FunctionCallee CStd::GCADDROOT_FUNC() const {
        return createCStdFuncDecl(
                "gc_add_root", codeMap->VOID_TYPE(),
                {codeMap->toLLVMClass("Object")->getPointerTo()->getPointerTo()},
                false);
    }

    llvm::FunctionCallee CStd::GCADDSTRINGROOT_FUNC() const {
        return createCStdFuncDecl(
                "gc_add_string_root", codeMap->VOID_TYPE(),
                {codeMap->toBasicType("String")->getPointerTo()}, false);
    }

    llvm::FunctionCallee CStd::GCREMOVEROOT_FUNC() const {
        return createCStdFuncDecl(
                "gc_remove_root", codeMap->VOID_TYPE(),
                {codeMap->toLLVMClass("Object")->getPointerTo()->getPointerTo()},
                false);
    }

    llvm::FunctionCallee CStd::GCREMOVESTRINGROOT_FUNC() const {
        return createCStdFuncDecl(
                "gc_remove_string_root", codeMap->VOID_TYPE(),
                {codeMap->toBasicType("String")->getPointerTo()}, false);
    }

    llvm::FunctionCallee CStd::GCSYSTEMINIT_FUNC() const {
        return createCStdFuncDecl("gc_system_init", "Void", {"Int"});
    }

    llvm::FunctionCallee CStd::GCSYSTEMDESTROY_FUNC() const {
        return createCStdFuncDecl("gc_system_destroy", "Void", {});
    }
} // CoolCompiler