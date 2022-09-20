//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_AST_H
#define COOLCOMPILER_AST_H

#include <string>

namespace CoolCompiler {

    class AST {
    private:
        std::string identifier;
    public:
        AST();
        explicit AST(const std::string &identifier);
        [[nodiscard]] std::string getIdentifier() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_AST_H
