//
// Created by luoli on 2022/10/16.
//

#ifndef COOLCOMPILER_CONSOLECOLORS_H
#define COOLCOMPILER_CONSOLECOLORS_H

#include <ostream>

namespace Color {

    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        explicit Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }

        static Modifier RED(bool foreground = false){
            return foreground ? Modifier(Code::FG_RED) : Modifier(Code::BG_RED);
        }

        static Modifier GREEN(bool foreground = false){
            return foreground ? Modifier(Code::FG_GREEN) : Modifier(Code::BG_GREEN);
        }

        static Modifier BLUE(bool foreground = false){
            return foreground ? Modifier(Code::FG_BLUE) : Modifier(Code::BG_BLUE);
        }

        static Modifier DEFAULT(bool foreground = false){
            return foreground ? Modifier(Code::FG_DEFAULT) : Modifier(Code::BG_DEFAULT);
        }
    };

} // Color

#endif //COOLCOMPILER_CONSOLECOLORS_H
