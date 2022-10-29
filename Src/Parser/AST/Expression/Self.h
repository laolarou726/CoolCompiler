//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_SELF_H
#define COOLCOMPILER_SELF_H

#include "Expression.h"

namespace CoolCompiler {

    class Self : public Expression {
    public:
        Self();

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            return "Self";
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "[SELF]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_SELF_H
