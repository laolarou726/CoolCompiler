//
// Created by luoli on 2022/9/18.
//

#include "MethodAccess.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    MethodAccess::MethodAccess(Expression* instance,
                               const std::string &method,
                               const std::vector<Expression*> &arguments) : Expression("method_access") {
        this->instance = instance;
        this->method = method;
        this->arguments = arguments;
    }

    Expression* MethodAccess::getInstance() const {
        return instance;
    }

    std::string MethodAccess::getMethod() const {
        return method;
    }

    std::vector<Expression*> MethodAccess::getArguments() const {
        return arguments;
    }

    std::string MethodAccess::typeCheck(SemanticAnalyzer *analyzer) {
        std::string exprType = instance->typeCheck(analyzer);

        if(!analyzer->isTypeDefined(exprType)){
            std::string message = fmt::format("Dispatch on undefined class [{}].", exprType);
            analyzer->fail(message);
        }

        auto* methodDef = analyzer->lookupMethod(exprType, method);

        if(methodDef == nullptr){
            std::string message = fmt::format("Dispatch to undefined method [{}].", method);
            analyzer->fail(message);

            return "Object";
        }

        std::string returnType = methodDef->getReturnType();
        auto formals = methodDef->getFormalArguments();
        auto declaredArgsCount = arguments.size();
        auto actualArgsCount = formals.size();

        if(declaredArgsCount != actualArgsCount){
            std::string message = fmt::format("In the dispatch to method [{}], given number of arguments ({}) differs from the declared method's number of arguments ({}).",
                                              method, actualArgsCount, declaredArgsCount);
            analyzer->fail(message);
        }

        int index = 0;
        bool isDispatchValid = true;

        while(index < arguments.size() && index < formals.size()){
            std::string declaredArgType = formals[index]->getType();
            declaredArgType = declaredArgType == "SELF_TYPE" ? analyzer->getCurrentClassName() : declaredArgType;

            std::string actualArgType = arguments[index]->typeCheck(analyzer);
            actualArgType = actualArgType == "SELF_TYPE" ? analyzer->getCurrentClassName() : actualArgType;

            if(!analyzer->isSubtype(actualArgType, declaredArgType)){
                isDispatchValid = false;

                std::string message = fmt::format("In the dispatch of the method [{}], type <{}> of provided argument [{}] is not compatible with the corresponding signature type <{}>.",
                                                  method, actualArgType, formals[index]->getName(), declaredArgType);
                analyzer->fail(message);
            }

            index++;
        }

        if(!isDispatchValid)
            return "Object";

        return returnType == "SELF_TYPE" ? exprType : returnType;
    }

    llvm::Value *MethodAccess::visit(CoolCompiler::CodeGenerator *generator) {
        std::vector<std::pair<llvm::Value*, std::string>> argGCRootsTypes;

        auto* builder = generator->getBuilder();
        auto* codeMap = generator->getCodeMap();
        auto* cStd = generator->getCStd();

        // codegen args first
        for (auto* arg : arguments) {
            llvm::Value* argValue = arg->visit(generator);
            std::string argExprType = arg->getExpressionType();

            // add GC roots for each arg e.g f(new A, new B) we don't want GC to delete
            // A when allocating B
            if (!SemanticAnalyzer::isPrimitive(argExprType) || argExprType == "String") {
                llvm::AllocaInst* alloca_inst = builder->CreateAlloca(
                        codeMap->LLVM_BASIC_OR_CLASS_PTR_TYPE(argExprType));
                builder->CreateStore(argValue, alloca_inst);

                if (argExprType == "String") {
                    builder->CreateCall(cStd->GCADDSTRINGROOT_FUNC(), {alloca_inst});
                    argGCRootsTypes.emplace_back(alloca_inst, argExprType);
                } else {
                    llvm::Value* root = generator->addGCRoot(alloca_inst);
                    argGCRootsTypes.emplace_back(root, argExprType);
                }
            }
        }
        // codegen LHS after all args
        llvm::Value* instanceValue = instance->visit(generator);
        std::string instanceExprType = instance->getExpressionType();

        // now it's safe to remove the gc roots for the args since there wont be any
        // more GC allocations until inside the called function body which will set
        // its own roots for its args
        for (auto it = argGCRootsTypes.rbegin();
             it != argGCRootsTypes.rend(); ++it) {
            if (it->second == "String") {
                builder->CreateCall(cStd->GCREMOVESTRINGROOT_FUNC(), {it->first});
            } else {
                builder->CreateCall(cStd->GCREMOVEROOT_FUNC(), {it->first});
            }
        }

        Class* class_calling_method =
                codeMap->getClass(instanceExprType);

        const bool is_static_dispatch = call_expr.GetStaticDispatchType().has_value();
        if (is_static_dispatch) {
            class_calling_method =
                    codeMap->getClass(call_expr.GetStaticDispatchType().value());
        }

        const auto method_feature =
                class_calling_method->GetMethodFeatureByName(method);

        Class* class_that_defines_method =
                class_calling_method->GetClassOrSuperClassThatDefinesMethod(
                        method_feature);

        if (!SemanticAnalyzer::isPrimitive(instanceExprType)) {
            GenExitIfVoid(instanceValue, call_expr.GetLhsExpr()->GetLineRange().end_line_num,
                          "Dispatch to void.");
        }

        std::vector<llvm::Value*> arg_vals;

        if (codeMap->getClass(instanceExprType) == class_that_defines_method) {
            arg_vals.push_back(instanceValue);
        } else {
            auto* dest_type =
                    codeMap->toLLVMClass(class_that_defines_method->getName())->getPointerTo();
            auto* casted_value = builder->CreateBitCast(instanceValue, dest_type);
            arg_vals.push_back(casted_value);
        }

        for (size_t i = 0; i < method_feature->GetArgs().size(); i++) {
            if (method_feature->GetArgs()[i].GetType() ==
                call_expr.GetArgs()[i]->GetExprType()) {
                arg_vals.push_back(call_expr.GetArgs()[i]->LlvmValue());
            } else {
                const auto dest_type =
                        codeMap->toLLVMClass(method_feature->GetArgs()[i].GetType())
                                ->getPointerTo();
                auto* casted_value = builder->CreateBitCast(
                        call_expr.GetArgs()[i]->LlvmValue(), dest_type);
                arg_vals.push_back(casted_value);
            }
        }

        if (is_static_dispatch) {
            const auto called_method = ast_to_.LlvmFunc(
                    call_expr.GetStaticDispatchType().value(), call_expr.GetMethodName());
            call_expr.SetLlvmValue(builder_.CreateCall(called_method, arg_vals));
        } else if (!AstToCodeMap::TypeUsesVtable(
                call_expr.GetLhsExpr()->GetExprType())) {
            const auto called_method = ast_to_.LlvmFunc(
                    call_expr.GetLhsExpr()->GetExprType(), call_expr.GetMethodName());
            call_expr.SetLlvmValue(builder_.CreateCall(called_method, arg_vals));
        } else {
            llvm::Value* vtable_ptr_ptr = builder_.CreateStructGEP(
                    ast_to_.LlvmClass(call_expr.GetLhsExpr()->GetExprType()), lhs_val,
                    AstToCodeMap::obj_vtable_index);
            llvm::Value* vtable_ptr = builder_.CreateLoad(vtable_ptr_ptr);
            const int method_offset_in_vtable =
                    ast_to_.GetVtable(class_calling_method)
                            .GetIndexOfMethodFeature(method_feature);
            llvm::Value* vtable_func_ptr =
                    builder_.CreateStructGEP(nullptr, vtable_ptr, method_offset_in_vtable);
            llvm::Value* vtable_func = builder_.CreateLoad(vtable_func_ptr);
            call_expr.SetLlvmValue(builder_.CreateCall(vtable_func, arg_vals));
        }

        llvm::Type* return_type =
                ast_to_.LlvmBasicOrClassPtrTy(call_expr.GetExprType());
        if (call_expr.LlvmValue()->getType() != return_type) {
            call_expr.SetLlvmValue(
                    builder_.CreateBitCast(call_expr.LlvmValue(), return_type));
        }

        return nullptr;
    }
} // CoolCompiler