//
// Created by luoli on 2022/11/13.
//

#ifndef COOLCOMPILER_CSTD_H
#define COOLCOMPILER_CSTD_H

#include <llvm/IR/Constants.h>

namespace CoolCompiler {

    class CodeMap;

    class CStd {
    private:
        CodeMap* codeMap;
        llvm::Module* module;

        [[nodiscard]] llvm::FunctionCallee createCStdFuncDecl(const std::string& func_name,
                                           const std::string& return_type_str,
                                           const std::vector<std::string>& arg_types,
                                           bool is_var_arg = false) const;
        [[nodiscard]] llvm::FunctionCallee createCStdFuncDecl(
                const std::string& func_name, llvm::Type* return_type,
                const std::vector<llvm::Type*>& llvm_arg_types,
                bool is_var_arg = false) const;
    public:
        [[nodiscard]] llvm::FunctionCallee PRINTF_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee STRLEN_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee STRCPY_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee STRNCPY_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee STRCAT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee EXIT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee STRCMP_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GETCHAR_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee ATOI_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCMALLOC_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCCOPYOBJ_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCMALLOCSTRING_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCADDROOT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCADDSTRINGROOT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCREMOVEROOT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCREMOVESTRINGROOT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCSYSTEMINIT_FUNC() const;
        [[nodiscard]] llvm::FunctionCallee GCSYSTEMDESTROY_FUNC() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CSTD_H
