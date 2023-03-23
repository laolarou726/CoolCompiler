//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ID_H
#define COOLCOMPILER_ID_H

#include "Expression.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class Id : public Expression {
    private:
        std::string name;
    public:
        Id();
        explicit Id(const std::string &name);
        [[nodiscard]] std::string getName() const;

        std::string typeCheck(SemanticAnalyzer* analyzer) override;
        llvm::Value * visit(CoolCompiler::CodeGenerator *generator) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << name << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_ID_H
