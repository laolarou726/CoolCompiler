//
// Created by luoli on 2022/9/18.
//

#include "If.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    If::If(Expression* condition, Expression* conditionTrue, Expression* conditionFalse) : Expression("if") {
        this->condition = condition;
        this->conditionTrue = conditionTrue;
        this->conditionFalse = conditionFalse;
    }

    Expression* If::getCondition() const {
        return condition;
    }

    Expression* If::getConditionTrue() const {
        return conditionTrue;
    }

    Expression* If::getConditionFalse() const {
        return conditionFalse;
    }

    std::string If::typeCheck(SemanticAnalyzer *analyzer) {
        std::string conditionType = condition->typeCheck(analyzer);
        std::string trueType = conditionTrue->typeCheck(analyzer);
        std::string falseType = conditionFalse->typeCheck(analyzer);

        trueType = trueType == "SELF_TYPE" ? analyzer->getCurrentClassName() : trueType;
        falseType = falseType == "SELF_TYPE" ? analyzer->getCurrentClassName() : falseType;

        if(conditionType != "Bool"){
            std::string message = fmt::format("{}: Expected the condition of if to be of type <Bool> but got the predicate of type <{}> instead.",
                                              "If", conditionType);
            analyzer->fail(message);
        }

        std::string result = analyzer->leastCommonAncestorType(trueType, falseType);
        return result;
    }

    llvm::Value *If::visit(CoolCompiler::CodeGenerator *generator) {
        auto* builder = generator->getBuilder();
        auto* context = generator->getContext();
        auto* codeMap = generator->getCodeMap();

        llvm::Value* conditionValue = condition->visit(generator);

        llvm::BasicBlock* thenBasicBlock =
                llvm::BasicBlock::Create(*context, "then", codeMap->getCurrentLLVMFunction());
        llvm::BasicBlock* elseBasicBlock = llvm::BasicBlock::Create(*context, "else");
        llvm::BasicBlock* doneBasicBlock = llvm::BasicBlock::Create(*context, "done-if");

        builder->CreateCondBr(conditionValue, thenBasicBlock, elseBasicBlock);

        builder->SetInsertPoint(thenBasicBlock);

        llvm::Value* thenValue = conditionTrue->visit(generator);
        thenValue = generator->covertValue(thenValue, conditionTrue->getExpressionType(),
                               getExpressionType());

        thenBasicBlock = builder->GetInsertBlock();

        builder->CreateBr(doneBasicBlock);

        codeMap->getCurrentLLVMFunction()->getBasicBlockList().push_back(elseBasicBlock);
        builder->SetInsertPoint(elseBasicBlock);

        llvm::Value* elseValue = conditionFalse->visit(generator);
        elseValue = generator->covertValue(elseValue, conditionFalse->getExpressionType(),
                               getExpressionType());

        elseBasicBlock = builder->GetInsertBlock();

        builder->CreateBr(doneBasicBlock);

        codeMap->getCurrentLLVMFunction()->getBasicBlockList().push_back(doneBasicBlock);
        builder->SetInsertPoint(doneBasicBlock);

        llvm::PHINode* pn = builder->CreatePHI(codeMap->LLVM_BASIC_OR_CLASS_PTR_TYPE(getExpressionType()), 2);
        pn->addIncoming(thenValue, thenBasicBlock);
        pn->addIncoming(elseValue, elseBasicBlock);

        return pn;
    }
} // CoolCompiler