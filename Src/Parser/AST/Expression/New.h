//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_NEW_H
#define COOLCOMPILER_NEW_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class New : public Expression {
    private:
        std::string type;
    public:
        explicit New(const std::string &type);
        [[nodiscard]] std::string getType() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "NEW [" << type << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_NEW_H
