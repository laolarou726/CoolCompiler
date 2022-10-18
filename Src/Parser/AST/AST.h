//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_AST_H
#define COOLCOMPILER_AST_H

#include <string>
#include <stdexcept>
#include <iostream>
#include "../../Utils/ConsoleColors.h"

namespace CoolCompiler {

    class AST {
    protected:
        std::string identifier;
    public:
        AST();
        explicit AST(const std::string &identifier);
        [[nodiscard]] std::string getIdentifier() const;
        virtual void print(int depth) = 0;

        static void printTab(int depth){
            setColor(depth);
            for(int i = 0; i < depth; i++)
                std::cout << "\t";
        }

        static void setColor(int depth){
            int colorIndex = depth % 3;
            Color::Modifier modifier = colorIndex == 0 ?
                                       Color::Modifier::RED(true)
                                       : colorIndex == 1 ?
                                         Color::Modifier::BLUE(true) :
                                         Color::Modifier::GREEN(true);

            std::cout << modifier;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_AST_H
