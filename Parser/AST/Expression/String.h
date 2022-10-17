//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_STRING_H
#define COOLCOMPILER_STRING_H

#include "Expression.h"

namespace CoolCompiler {

    class String : public Expression {
    private:
        std::string value;
    public:
        explicit String(const std::string &value);
        [[nodiscard]] std::string getValue() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "[STRING \"" << value << "\"]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_STRING_H
