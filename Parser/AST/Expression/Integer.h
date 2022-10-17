//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_INTEGER_H
#define COOLCOMPILER_INTEGER_H

#include "Expression.h"

namespace CoolCompiler {

    class Integer : public Expression {
    private:
        int value;
    public:
        explicit Integer(int value);
        [[nodiscard]] int getValue() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << value << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_INTEGER_H
