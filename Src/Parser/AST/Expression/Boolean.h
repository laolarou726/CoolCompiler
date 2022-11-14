//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_BOOLEAN_H
#define COOLCOMPILER_BOOLEAN_H

#include "Expression.h"

namespace CoolCompiler {

    class Boolean : public Expression {
    private:
        bool value;
    public:
        explicit Boolean(bool value);
        [[nodiscard]] bool getValue() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << (value ? "TRUE" : "FALSE") << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_BOOLEAN_H
