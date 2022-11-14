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

        llvm::BasicBlock* loop_cond_bb =
                llvm::BasicBlock::Create(context, "loop-cond", codeMap.CurLlvmFunc());

        llvm::BasicBlock* loop_body_bb =
                llvm::BasicBlock::Create(context, "loop-body", codeMap.CurLlvmFunc());

        llvm::BasicBlock* loop_done_bb =
                llvm::BasicBlock::Create(context, "loop-done", codeMap.CurLlvmFunc());

        builder->CreateBr(loop_cond_bb);

        builder->SetInsertPoint(loop_cond_bb);
        getCondition()->Accept(*this);
        llvm::Value* cond_val = while_expr.GetConditionExpr()->LlvmValue();
        builder->CreateCondBr(cond_val, loop_body_bb, loop_done_bb);

        builder->SetInsertPoint(loop_body_bb);
        while_expr.GetLoopExpr()->Accept(*this);
        builder->CreateBr(loop_cond_bb);

        builder->SetInsertPoint(loop_done_bb);

        while_expr.SetLlvmValue(LlvmDefaultVal("Object"));
    }
} // CoolCompiler