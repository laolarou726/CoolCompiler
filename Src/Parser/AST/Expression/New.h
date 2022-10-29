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

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            if(type != "SELF_TYPE" && !analyzer->isTypeDefined(type)){
                std::string message = fmt::format("{}: Tried to instantiate an object of undefined type: {}.",
                                                  "New", type);
                analyzer->fail(message);

                return "Object";
            }

            return type;
        }

        void print(int depth) override{
            printTab(depth);
            std::cout << "NEW [" << type << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_NEW_H
