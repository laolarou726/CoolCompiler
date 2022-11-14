//
// Created by luoli on 2022/11/13.
//

#ifndef COOLCOMPILER_VTABLE_H
#define COOLCOMPILER_VTABLE_H

#include <llvm/IR/DerivedTypes.h>
#include "../Parser/AST/Class.h"

namespace CoolCompiler {

    class VTable {
    private:
        Class* class_;
        llvm::StructType* structType;
        llvm::GlobalValue* globalInstance = nullptr;
        std::vector<llvm::Constant*> functions;
    public:
        VTable(llvm::LLVMContext& context, Class* class_);
        void build(llvm::Module* module, const std::vector<llvm::Constant*>& tableFunctions);
        size_t getIndexOfMethodFeature(const std::vector<Class*>& inheritList, const FeatureMethod* method) const;

        [[nodiscard]] llvm::StructType* getStructType() const;
        [[nodiscard]] llvm::GlobalValue* getGlobalInstance() const;
        [[nodiscard]] const std::vector<llvm::Constant*>& getFunctions() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_VTABLE_H
