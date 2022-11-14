//
// Created by luoli on 2022/11/13.
//

#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include "VTable.h"
#include "fmt/format.h"
#include "../Parser/AST/Feature/FeatureAttribute.h"

namespace CoolCompiler {
    VTable::VTable(llvm::LLVMContext &context, Class *class_) {
        this->class_ = class_;
        this->structType = llvm::StructType::create(context, class_->getName() + "-vtable");
    }

    void VTable::build(llvm::Module *module, const std::vector<llvm::Constant *> &tableFunctions) {
        std::vector<llvm::Type*> vtable_method_types;
        vtable_method_types.reserve(tableFunctions.size());
        for (auto func : tableFunctions) {
            vtable_method_types.push_back(func->getType());
        }

        structType->setBody(vtable_method_types);

        module->getOrInsertGlobal(class_->getName() + "-vtable-global",
                                  structType);
        llvm::GlobalVariable* vtable =
                module->getNamedGlobal(class_->getName() + "-vtable-global");
        vtable->setConstant(true);
        vtable->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
        vtable->setInitializer(
                llvm::ConstantStruct::get(structType, tableFunctions));

        functions = tableFunctions;
        globalInstance = vtable;
    }

    size_t VTable::getIndexOfMethodFeature(const std::vector<Class*>& inheritList, const FeatureMethod *method) const {
        std::set<std::string> already_seen_method_ids;
        size_t vtable_method_index = 0;

        for (const auto* superClass : inheritList) {
            for (auto* feature : superClass->getFeatures()) {
                if(dynamic_cast<FeatureAttribute*>(feature) != nullptr)
                    continue;

                auto* classMethod = (FeatureMethod*) feature;

                if (classMethod->getName() == method->getName()) {
                    return vtable_method_index;
                }
                if (already_seen_method_ids.find(classMethod->getName()) ==
                    already_seen_method_ids.end()) {
                    vtable_method_index++;
                }
                already_seen_method_ids.insert(classMethod->getName());
            }
        }

        std::string message = fmt::format("Method: [{}] not defined in class: <{}> or its super classes.",
                                          method->getName(),
                                          class_->getName());
        throw std::invalid_argument(message);
    }

    llvm::StructType *VTable::getStructType() const {
        return structType;
    }

    llvm::GlobalValue *VTable::getGlobalInstance() const {
        return globalInstance;
    }

    const std::vector<llvm::Constant *> &VTable::getFunctions() const {
        return functions;
    }
} // CoolCompiler