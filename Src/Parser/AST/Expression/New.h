//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_NEW_H
#define COOLCOMPILER_NEW_H

#include "Expression.h"

namespace CoolCompiler {

    class New : public Expression {
    private:
        std::string type;
    public:
        explicit New(const std::string &type);
        [[nodiscard]] std::string getType() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "NEW [" << type << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_NEW_H
