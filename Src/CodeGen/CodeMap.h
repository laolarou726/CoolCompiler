//
// Created by luoli on 2022/11/13.
//

#ifndef COOLCOMPILER_CODEMAP_H
#define COOLCOMPILER_CODEMAP_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <map>
#include "../Parser/AST/Class.h"
#include "../Parser/AST/Program.h"
#include "VTable.h"

namespace CoolCompiler {

    class CodeMap {
    private:
        std::map<Class*, llvm::StructType*> types;
        std::map<Class*, VTable> vtables;
        std::map<Class*, llvm::Function*> constructors;
        std::map<Class*, llvm::Function*> copyConstructors;
        std::map<FeatureMethod*, llvm::Function*> functions;

        llvm::StructType* GC_PTRS_INFO_TYPE = nullptr;

        llvm::LLVMContext* context;
        llvm::Module* module;
        llvm::IRBuilder<>* builder;
        Class* currentClass;
        SemanticAnalyzer* analyzer;

        llvm::FunctionType* getConstructorFunctionType(const Class* class_);
        llvm::FunctionType* getCopyConstructorFunctionType();

        llvm::Type* LLVM_BASIC_OR_CLASS_PTR_TYPE(const std::string& type_name);
        [[nodiscard]] bool needVTable(const std::string &type) const;
        [[nodiscard]] Class* getClass(const std::string &name) const;

        void addCopyConstructor(Class* class_);
        void addAttributes(Class* class_);
        void addMethods(Class* class_);
        void addConstructor(Class* class_);
        void insert(Class* class_);
    public:
        CodeMap(llvm::LLVMContext* context,
                llvm::Module* module,
                llvm::IRBuilder<>* builder,
                SemanticAnalyzer* analyzer);

        [[nodiscard]] llvm::Type* VOID_TYPE() const{
            return builder->getVoidTy();
        }

        [[nodiscard]] llvm::ConstantInt* toLLVMInt32(int num) const;
        [[nodiscard]] llvm::Type* toBasicType(const std::string &class_) const;
        [[nodiscard]] llvm::StructType* toLLVMClass(const std::string& className);
        void setCurrentClass(Class* class_);

        static constexpr int GC_PTRS_COUNT_INDEX = 0;
        static constexpr int GC_PTRS_ARRAY_INDEX = 1;
        static constexpr int GC_STR_COUNT_INDEX = 2;
        static constexpr int GC_STRS_ARRAY_INDEX = 3;
        static constexpr int GC_PTRS_NEXT_INDEX = 4;

        static constexpr int OBJ_GC_NEXT_OBJ_INDEX = 0;
        static constexpr int OBJ_GC_PREV_OBJ_INDEX = 1;
        static constexpr int OBJ_GC_IS_REACHABLE_INDEX = 2;

        static constexpr int OBJ_TYPENAME_INDEX = 3;

        static constexpr int OBJ_VTABLE_INDEX = 4;
        static constexpr int OBJ_TYPESIZE_INDEX = 5;
        // need a pointer to constructor to handle "new SELF_TYPE"
        static constexpr int OBJ_CONSTRUCTOR_INDEX = 6;
        // need copy_constructor for copy method
        static constexpr int OBJ_copy_CONSTRUCTOR_INDEX = 7;
        static constexpr int OBJ_boxed_DATA_INDEX = 8;

        // attributes start after the things above
        static constexpr int OBJ_attributes_OFFSET = 9;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CODEMAP_H
