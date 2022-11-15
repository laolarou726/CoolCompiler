//
// Created by luoli on 2022/9/18.
//

#include "Tilde.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Tilde::Tilde(Expression *expression) : Expression("tilde") {
        this->expression = expression;
    }

    Expression* Tilde::getExpression() const {
        return expression;
    }

    std::string Tilde::typeCheck(SemanticAnalyzer *analyzer) {
        std::string type = expression->typeCheck(analyzer);

        if(type != "Int"){
            std::string message = fmt::format("{}: Argument of the operator '~' has type <{}> instead of Int.",
                                              "Tilde", type);
            return "Object";
        }

        return "Int";
    }

    llvm::Value *Tilde::visit(CoolCompiler::CodeGenerator *generator) {
        CodeMap* codeMap = generator->getCodeMap();
        llvm::Value* exprVal = expression->visit(generator);

        return generator->getBuilder()->CreateMul(codeMap->toLLVMInt32(-1), exprVal);
    }
} // CoolCompiler