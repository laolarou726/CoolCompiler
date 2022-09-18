//
// Created by luoli on 2022/9/14.
//

#ifndef COOLCOMPILER_LEXER_H
#define COOLCOMPILER_LEXER_H

#include <string>
#include <vector>
#include "Token.h"
#include "Error.h"
#include <map>

namespace CoolCompiler {

    class Lexer {
    private:
        std::vector<Token> tokens = {};
        std::vector<Error> errors = {};

        bool skipBlank();

        bool isLiteral();

        bool isSingleCharacterToken();

        bool isString(int &pos, std::string &lexeme);

        bool isNumber(int &pos, std::string &lexeme);

        bool isTypeId(int &pos, std::string &lexeme);

        bool isObjId(int &pos, std::string &lexeme);

        static bool isSelf(std::string &lexeme);

        static bool isKeyword(std::string &lexeme);

        static bool isSelfType(std::string &lexeme);

        void moveNext(int &pos, std::string &lexeme);

        void addToken(const TokenType &tokenType, std::string &lexeme);

        std::string source;
        int sourcePosition;
        int contextPosition;
        int lineNumber;
    public:
        explicit Lexer(const std::string &filePath);

        std::string getSource();

        void doScan();
    };

} // CoolCompiler

#endif //COOLCOMPILER_LEXER_H
