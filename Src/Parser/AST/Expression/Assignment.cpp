//
// Created by luoli on 2022/9/18.
//

#include "Assignment.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Assignment::Assignment(Id* id, Expression* expression) : Expression("assignment")  {
        this->id = id;
        this->expression = expression;
    }

    Id* Assignment::getId() const {
        return id;
    }

    Expression* Assignment::getExpression() const {
        return expression;
    }

    std::string Assignment::typeCheck(SemanticAnalyzer *analyzer) {
        std::string name = id->getName();

        if(name == "Self"){
            std::string message = fmt::format("{}: Cannot assign to 'self'.", "Assignment");
            analyzer->fail(message);
        }

        std::string* idType = analyzer->getObjectsTable()->lookup(name);

        if(idType == nullptr){
            std::string message = fmt::format("{}: Tried to assign undeclared identifier [{}].",
                                              "Assignment", name);
            analyzer->fail(message);

            return "Object";
        }

        std::string exprType = expression->typeCheck(analyzer);

        bool isAssignValid = analyzer->isSubtype(exprType, *idType);

        if(!isAssignValid){
            std::string message = fmt::format("{}: The identifier [{}] has been declared as <{}> but assigned with incompatible type <{}>.",
                                              "Assignment", name, *idType, exprType);
            analyzer->fail(message);

            return "Object";
        }

        return exprType;
    }

    llvm::Value *Assignment::visit(CoolCompiler::CodeGenerator *generator) {
        auto* builder = generator->getBuilder();

        std::string exprType = expression->getExpressionType();
        llvm::Value* resultValue = expression->visit(generator);
        llvm::Value* assign_lhs_ptr = getAssignmentLhsPtr(generator);

        if (SemanticAnalyzer::isPrimitive(exprType)) {
            auto* lhs_ty = assign_lhs_ptr->getType()->getPointerElementType();

            const bool lhs_is_basic_ty = lhs_ty == builder->getInt1Ty() ||
                                         lhs_ty == builder->getInt32Ty() ||
                                         lhs_ty == builder->getInt8PtrTy();

            if (!lhs_is_basic_ty) {
                resultValue = generator->covertValue(resultValue, exprType, "Object");
            }
        } else if (assign_lhs_ptr->getType()->getPointerElementType() != resultValue->getType()) {
            resultValue = builder->CreateBitCast(
                    resultValue, assign_lhs_ptr->getType()->getPointerElementType());
        }

        builder->CreateStore(resultValue, assign_lhs_ptr);

        return resultValue;
    }

    llvm::Value *Assignment::getAssignmentLhsPtr(CoolCompiler::CodeGenerator *generator) {
        /*
        const auto in_scope_var = in_scope_vars_.find(assign.GetId());
        if (in_scope_var != in_scope_vars_.end()) {
            return in_scope_var->second.top();
        }
        */

        auto* analyzer = generator->getAnalyzer();
        auto* codeMap = generator->getCodeMap();
        auto* builder = generator->getBuilder();

        for (Class* inheritClass : analyzer->getInheritList(codeMap->getCurrentClass())) {
            for (FeatureBase* feature : inheritClass->getFeatures()) {
                if(dynamic_cast<FeatureMethod*>(feature) != nullptr)
                    continue;

                auto* attr = (FeatureAttribute*) feature;

                llvm::Value* cur_class_val = codeMap->getCurrentLLVMFunction()->args().begin();
                cur_class_val = generator->covertValue(cur_class_val, codeMap->getCurrentClass()->getName(),
                                            inheritClass->getName());

                if (attr->getName() == id->getName()) {
                    return builder->CreateStructGEP(codeMap->toLLVMClass(inheritClass->getName()),
                                                    cur_class_val,
                                                    StructAttrIndex(inheritClass, attr));
                }
            }
        }

        return nullptr;
    }
} // CoolCompiler