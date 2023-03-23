//
// Created by luoli on 2022/11/13.
//

#include "CodeMap.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    llvm::Type *CodeMap::toBasicType(const std::string &class_) const {
        std::string resultClass = class_;

        if(class_ == "SELF_TYPE")
            resultClass = this->currentClass->getName();

        if(resultClass == "Int")
            return builder->getInt32Ty();
        if(resultClass == "String")
            return builder->getInt8PtrTy();
        if(resultClass == "Bool")
            return builder->getInt1Ty();

        return nullptr;
    }

    void CodeMap::setCurrentClass(Class *class_) {
        this->currentClass = class_;
    }

    llvm::StructType *CodeMap::toLLVMClass(const std::string &className) {
        auto* class_ = getClass(className);

        return types[class_];
    }

    Class *CodeMap::getClass(const std::string &name) const {
        std::string resultName = name;

        if(name == "SELF_TYPE")
            resultName = this->currentClass->getName();

        for(auto* class_ : *analyzer->getProgram()->getClasses()){
            if(class_->getName() == resultName)
                return class_;
        }

        return nullptr;
    }

    bool CodeMap::needVTable(const std::string &type) const {
        return !(type == "Int" ||
                type == "String" ||
                type == "Bool");
    }

    void CodeMap::addMethods(Class *class_) {
        currentClass = class_;

        std::vector<llvm::Constant*> vtableFunctions;
        auto* parentClass = analyzer->getParentClass(class_->getName());

        if (parentClass != nullptr) {
            const std::vector<llvm::Constant*>& superVtableFunctions = vtables.at(parentClass).getFunctions();

            vtableFunctions.insert(vtableFunctions.end(),
                                   superVtableFunctions.begin(),
                                   superVtableFunctions.end());
        }

        for (auto* feature : class_->getFeatures()) {
            if(dynamic_cast<FeatureAttribute*>(feature) != nullptr)
                continue;

            auto* method = (FeatureMethod*) feature;

            llvm::Type* returnType = LLVM_BASIC_OR_CLASS_PTR_TYPE(method->getReturnType());
            std::vector<llvm::Type*> argTypes;

            // first param is always implicit 'self'
            argTypes.emplace_back(LLVM_BASIC_OR_CLASS_PTR_TYPE(class_->getName()));
            for (auto& arg : method->getFormalArguments()) {
                argTypes.emplace_back(LLVM_BASIC_OR_CLASS_PTR_TYPE(arg->getType()));
            }

            // use _ for basic type symbols names, so we can define them in C code
            // use - for non-basic types to avoid conflict with other Type/Method combos
            std::string funcSymbolName =
                    analyzer->isPrimitive(class_->getName())
                    ? class_->getName() + "_" + method->getName()
                    : class_->getName() + "-" + method->getName();

            llvm::FunctionType* funcType =
                    llvm::FunctionType::get(returnType, argTypes, false);
            llvm::Function* func = llvm::Function::Create(
                    funcType, llvm::Function::ExternalLinkage, funcSymbolName, module);
            functions[method] = func;

            if (needVTable(class_->getName())) {
                const size_t vtableMethodIndex =
                        vtables.at(class_).getIndexOfMethodFeature(analyzer->getInheritList(class_), method);
                if (vtableMethodIndex < vtableFunctions.size()) {
                    // redefining a super method
                    vtableFunctions[vtableMethodIndex] = func;
                } else {
                    assert(vtableMethodIndex == vtableFunctions.size());
                    vtableFunctions.emplace_back(func);
                }
            }
        }

        if (needVTable(class_->getName())) {
            vtables.at(class_).build(module, vtableFunctions);
        }

        currentClass = nullptr;
    }

    void CodeMap::addConstructor(Class *class_) {
        using namespace std::string_literals;

        llvm::FunctionType* constructorFuncType = getConstructorFunctionType(class_);
        llvm::Function* constructor = llvm::Function::Create(
                constructorFuncType, llvm::Function::ExternalLinkage,
                "construct"s + "-" + class_->getName(), module);

        constructors.emplace(class_, constructor);

        addCopyConstructor(class_);
    }

    void CodeMap::addAttributes(Class *class_) {
        currentClass = class_;

        std::vector<llvm::Type*> classAttributes;
        // obj_gc_next_obj_index
        classAttributes.emplace_back(toLLVMClass("Object")->getPointerTo());
        // obj_gc_prev_obj_index
        classAttributes.emplace_back(toLLVMClass("Object")->getPointerTo());
        // obj_gc_is_reachable_index
        // use an i8 instead of i1 since clang emits i8 for bool
        // i1 should work too since i8 is min addressable data but use i8 to be safe
        classAttributes.emplace_back(builder->getInt8Ty());

        // obj_typename_index
        classAttributes.emplace_back(builder->getInt8PtrTy());

        // obj_vtable_index
        classAttributes.emplace_back(
                vtables.at(class_).getStructType()->getPointerTo());
        // obj_typesize_index (to allow copying without a different function per type)
        classAttributes.emplace_back(builder->getInt32Ty());
        // obj_constructor_index (to support "new SELF_TYPE")
        classAttributes.emplace_back(
                getConstructorFunctionType(class_)->getPointerTo());
        // obj_copy_constructor_index
        classAttributes.emplace_back(getCopyConstructorFunctionType()->getPointerTo());
        // obj_boxed_data_index
        classAttributes.emplace_back(builder->getInt8PtrTy());

        for (Class* inheritClass : analyzer->getInheritList(class_)) {
            // Put the gc ptrs info before attributes, so we know how many ptrs to visit
            // for GC and where to start visiting for the next super class
            classAttributes.emplace_back(GC_PTRS_INFO_TYPE);

            std::vector<FeatureAttribute*> sortedAttrs;
            for(auto* feature : inheritClass->getFeatures()){
                if(dynamic_cast<FeatureMethod*>(feature) != nullptr)
                    continue;

                auto* attr = (FeatureAttribute*) feature;

                if (!analyzer->isPrimitive(attr->getType()))
                    sortedAttrs.emplace_back(attr);
            }

            for(auto* feature : inheritClass->getFeatures()){
                if(dynamic_cast<FeatureMethod*>(feature) != nullptr)
                    continue;

                auto* attr = (FeatureAttribute*) feature;

                if (attr->getType() == "String")
                    sortedAttrs.emplace_back(attr);
            }

            for(auto* feature : inheritClass->getFeatures()){
                if(dynamic_cast<FeatureMethod*>(feature) != nullptr)
                    continue;

                auto* attr = (FeatureAttribute*) feature;

                if (attr->getType() != "String" && analyzer->isPrimitive(attr->getType())) {
                    sortedAttrs.emplace_back(attr);
                }
            }

            // Put non-basic attrs first, so we can visit them for GC
            for (auto* attr : sortedAttrs) {
                llvm::Type* attrType = LLVM_BASIC_OR_CLASS_PTR_TYPE(attr->getType());
                classAttributes.push_back(attrType);
            }
        }

        types.at(class_)->setBody(classAttributes);

        currentClass = nullptr;
    }

    void CodeMap::addCopyConstructor(Class *class_) {
        using namespace std::string_literals;

        llvm::FunctionType* copyConstructorFuncType =
                getCopyConstructorFunctionType();

        llvm::Function* constructor = llvm::Function::Create(
                copyConstructorFuncType, llvm::Function::ExternalLinkage,
                "copy-construct"s + "-" + class_->getName(), module);

        copyConstructors.emplace(class_, constructor);
    }

    CodeMap::CodeMap(llvm::LLVMContext *context,
                     llvm::Module *module,
                     llvm::IRBuilder<> *builder,
                     SemanticAnalyzer* analyzer) {
        this->context = context;
        this->module = module;
        this->builder = builder;
        this->analyzer = analyzer;

        for (auto* innerClass : *analyzer->getProgram()->getClasses()) {
            insert(innerClass);
        }

        insert(analyzer->GET_IO_CLASS());
        insert(analyzer->GET_OBJECT_CLASS());

        GC_PTRS_INFO_TYPE = llvm::StructType::create(*context, "GcPtrsInfo");
        GC_PTRS_INFO_TYPE->setBody(
                {builder->getInt32Ty(),
                 toLLVMClass("Object")->getPointerTo()->getPointerTo(),
                 builder->getInt32Ty(), toBasicType("String")->getPointerTo(),
                 GC_PTRS_INFO_TYPE->getPointerTo()});
    }

    void CodeMap::insert(Class* class_) {
        types.emplace(class_, llvm::StructType::create(*context, class_->getName()));
        vtables.emplace(class_, VTable(*context, class_));
    }

    llvm::Type *CodeMap::LLVM_BASIC_OR_CLASS_PTR_TYPE(const std::string &type_name) {
        llvm::Type* type = toBasicType(type_name);

        if (type == nullptr) {
            type = toLLVMClass(type_name)->getPointerTo();
        }

        return type;
    }

    llvm::FunctionType *CodeMap::getConstructorFunctionType(const Class* class_) {
        return llvm::FunctionType::get(builder->getVoidTy(), {toLLVMClass(class_->getName())->getPointerTo()}, false);
    }

    llvm::FunctionType *CodeMap::getCopyConstructorFunctionType() {
        return llvm::FunctionType::get(builder->getVoidTy(),
                                       {toLLVMClass("Object")->getPointerTo(),
                                        toLLVMClass("Object")->getPointerTo()},
                                        false);
    }

    llvm::ConstantInt *CodeMap::toLLVMInt32(int num) const {
        return llvm::ConstantInt::get(*context, llvm::APInt(32, num, true));
    }

    llvm::Function *CodeMap::getCurrentLLVMFunction() {
        return functions.at(currentMethod);
    }

    llvm::Value *CodeMap::getLLVMDefault(const std::string &type) {
        if (type == "Int")
            return toLLVMInt32(0);

        if (type == "Bool")
            return llvm::ConstantInt::get(*context, llvm::APInt(1, 0, false));

        if (type == "String") {
            llvm::Value* malloc_val = builder->CreateCall(cStd->GCMALLOCSTRING_FUNC(), {toLLVMInt32(1)});
            llvm::Value* str_global = builder->CreateGlobalStringPtr("");
            builder->CreateCall(cStd->STRCPY_FUNC(), {malloc_val, str_global});

            return malloc_val;
        }

        return llvm::ConstantPointerNull::get(toLLVMClass(type)->getPointerTo());
    }

    Class *CodeMap::getCurrentClass() const {
        return currentClass;
    }

    llvm::Function *CodeMap::getConstructor(const std::string &type) {
        return constructors.at(getClass(type));
    }

    llvm::Function *CodeMap::getLLVMFunction(const std::string &className, const std::string &methodName) {
        std::vector<FeatureBase*> features = getClass(className)->getFeatures();
        FeatureMethod* method = nullptr;

        for(auto* feature : features){
            if(dynamic_cast<FeatureAttribute*>(feature) != nullptr)
                continue;

            auto* curMethod = (FeatureMethod*) feature;

            if(method->getName() == methodName){
                method = curMethod;
                break;
            }
        }

        if(method == nullptr) return nullptr;

        return functions.at(method);
    }

    unsigned long CodeMap::getGcPtrsInfoIndex(Class* class_) {
        unsigned long superAttrCount = 0;
        std::vector<Class*> superClasses = analyzer->getInheritList(class_);

        for (auto* super : superClasses) {
            int count = 0;

            for(FeatureBase* feature : super->getFeatures()){
                if(dynamic_cast<FeatureMethod*>(feature) != nullptr) continue;

                count++;
            }

            superAttrCount += count;
        }

        auto gcPtrInfoOffset = superClasses.size();

        return superAttrCount + gcPtrInfoOffset + CodeMap::OBJ_ATTRIBUTES_OFFSET;
    }

} // CoolCompiler