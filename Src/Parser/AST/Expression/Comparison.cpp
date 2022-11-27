//
// Created by luoli on 2022/9/18.
//

#include "Comparison.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Comparison::Comparison(const std::string &token, TokenType operation, Expression *expressionLeft, Expression *expressionRight)
    : Expression("comparison") {
        this->token = token;
        this->operation = operation;
        this->expressionLeft = expressionLeft;
        this->expressionRight = expressionRight;
    }

    TokenType Comparison::getOperation() const {
        return operation;
    }

    Expression* Comparison::getExpressionLeft() const {
        return expressionLeft;
    }

    Expression* Comparison::getExpressionRight() const {
        return expressionRight;
    }

    std::string Comparison::eqCheck(SemanticAnalyzer *analyzer) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);
        bool isLeftTypePrimitive = analyzer->isPrimitive(leftExprType);
        bool isRightTypePrimitive = analyzer->isPrimitive(rightExprType);

        if((isLeftTypePrimitive && isRightTypePrimitive) && leftExprType!= rightExprType){
            analyzer->fail("Illegal comparison with a basic type.");
        }

        return "Bool";
    }

    std::string Comparison::boolOpCheck(SemanticAnalyzer *analyzer, const std::string &symbol) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);

        if(leftExprType == "Int" && rightExprType == "Int")
            return "Bool";

        std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                          "Comparison", symbol, leftExprType, rightExprType);
        analyzer->fail(message);

        return "Object";
    }

    std::string Comparison::typeCheck(SemanticAnalyzer *analyzer) {
        switch (operation) {
            case LT:
                return boolOpCheck(analyzer, "<");
            case GT:
                return boolOpCheck(analyzer, ">");
            case LTOE:
                return boolOpCheck(analyzer, "<=");
            case GTOE:
                return boolOpCheck(analyzer, ">=");
            case EQ:
                return eqCheck(analyzer);
        }
    }

    llvm::Value *Comparison::getLLVMEq(SemanticAnalyzer* analyzer,
                                       CoolCompiler::CodeGenerator *generator,
                                       llvm::Value* lhs,
                                       llvm::Value* rhs) {
        auto* builder = generator->getBuilder();
        auto* codeMap = generator->getCodeMap();
        auto* cStd = generator->getCStd();

        std::string lhsType = expressionLeft->getExpressionType();
        std::string rhsType = expressionRight->getExpressionType();

        bool both_non_basic = !analyzer->isPrimitive(lhsType) && !analyzer->isPrimitive(rhsType);
        bool both_string = lhsType == "String" && rhsType == "String";

        llvm::AllocaInst* alloca_inst = builder->CreateAlloca(codeMap->LLVM_BASIC_OR_CLASS_PTR_TYPE(lhsType));
        builder->CreateStore(lhs, alloca_inst);

        llvm::Value* root = nullptr;
        if (both_non_basic) {
            root = generator->addGCRoot(alloca_inst);
        } else if (both_string) {
            builder->CreateCall(cStd->GCADDSTRINGROOT_FUNC(), {alloca_inst});
        }

        if (both_non_basic) {
            builder->CreateCall(cStd->GCREMOVEROOT_FUNC(), {root});
        } else if (both_string) {
            builder->CreateCall(cStd->GCREMOVESTRINGROOT_FUNC(), {alloca_inst});
        }

        if (lhsType == "Int" || lhsType == "Bool") {
            return builder->CreateICmpEQ(lhs, rhs);
        }

        if (lhsType == "String") {
            return generator->getLLVMStrEqCmp(lhs, rhs);
        }

        return builder->CreateICmpEQ(
                builder->CreatePtrToInt(lhs, builder->getInt32Ty()),
                builder->CreatePtrToInt(rhs, builder->getInt32Ty()));
    }

    llvm::Value *Comparison::visit(CoolCompiler::CodeGenerator *generator) {
        llvm::Value* leftExprValue = expressionLeft->visit(generator);
        llvm::Value* rightExprValue = expressionRight->visit(generator);
        auto* builder = generator->getBuilder();

        switch (operation) {
            case LT:
                return builder->CreateICmpSLT(leftExprValue, rightExprValue);
            case GT:
                return builder->CreateICmpSGT(leftExprValue, rightExprValue);
            case LTOE:
                return builder->CreateICmpSLE(leftExprValue, rightExprValue);
            case GTOE:
                return builder->CreateICmpSGE(leftExprValue, rightExprValue);
            case EQ:
                return getLLVMEq(new SemanticAnalyzer(), generator, leftExprValue, rightExprValue);
        }
    }
} // CoolCompiler