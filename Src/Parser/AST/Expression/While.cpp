//
// Created by luoli on 2022/9/18.
//

#include "While.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"
#include <llvm/IR/BasicBlock.h>

namespace CoolCompiler {
    While::While(Expression* condition, Expression* body) : Expression("while") {
        this->condition = condition;
        this->body = body;
    }

    Expression* While::getCondition() const {
        return condition;
    }

    Expression* While::getBody() const {
        return body;
    }

    std::string While::typeCheck(SemanticAnalyzer *analyzer) {
        std::string conditionType = condition->typeCheck(analyzer);
        body->typeCheck(analyzer);

        if(conditionType != "Bool"){
            std::string message = fmt::format("{}: Expected the condition of while to be of type <Bool> but got the predicate of type <{}> instead.",
                                              "While", conditionType);
            analyzer->fail(message);
        }

        return "Object";
    }

    llvm::Value *While::visit(CoolCompiler::CodeGenerator *generator) {
        auto* context = generator->getContext();
        auto* codeMap = generator->getCodeMap();
        auto* builder = generator->getBuilder();

        llvm::BasicBlock* loopConditionBlock =
                llvm::BasicBlock::Create(*context, "loop-cond", codeMap->getCurrentLLVMFunction());
        llvm::BasicBlock* loopBodyBlock =
                llvm::BasicBlock::Create(*context, "loop-body", codeMap->getCurrentLLVMFunction());
        llvm::BasicBlock* loopDoneBlock =
                llvm::BasicBlock::Create(*context, "loop-done", codeMap->getCurrentLLVMFunction());

        builder->CreateBr(loopConditionBlock);
        builder->SetInsertPoint(loopConditionBlock);

        llvm::Value* cond_val = getCondition()->visit(generator);
        builder->CreateCondBr(cond_val, loopBodyBlock, loopDoneBlock);

        builder->SetInsertPoint(loopBodyBlock);
        body->visit(generator);
        builder->CreateBr(loopConditionBlock);

        builder->SetInsertPoint(loopDoneBlock);

        return codeMap->getLLVMDefault("Object");
    }
} // CoolCompiler