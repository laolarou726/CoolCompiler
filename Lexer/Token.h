//
// Created by luoli on 2022/9/14.
//

#ifndef COOLCOMPILER_TOKEN_H
#define COOLCOMPILER_TOKEN_H

#include <string>
#include <unordered_map>

namespace CoolCompiler {

    enum TokenType {
        // Literals
        STRING = 1, NUMBER = 2, TYPE_ID = 3, OBJ_ID = 4,
        SELF = 5, SELF_TYPE = 6,

        // Single-or-2-character tokens
        LEFT_PAREN = 7, RIGHT_PAREN = 8, LEFT_BRACE = 9,
        RIGHT_BRACE = 10, COMMA = 11, DOT = 12, MINUS = 13,
        PLUS = 14, COLON = 15, SEMICOLON = 16, SLASH = 17, STAR = 18,
        EQ = 19, LT = 20, GT = 21, LTOE = 22, GTOE = 23, ASSIGN = 24,
        TILDE = 25, AT = 26,

        // Keywords, keywords are case-insensitive (except for true and false)
        CLASS = 27, ELSE = 28, FALSE = 29, FI = 30,
        IF = 31, IN = 32, INHERITS = 33, ISVOID = 34,
        LET = 35, LOOP = 36, POOL = 37, THEN = 38,
        WHILE = 39, CASE = 40, ESAC = 41, NEW = 42,
        OF = 43, NOT = 44, TRUE = 45, E0F = 46
    };

    class Token {
    private:
        TokenType tokenType;
        std::string lexeme;
        int contextPosition;
        int lineNumber;

    public:
        Token(TokenType tokenType, const std::string &lexeme, int contextPosition, int lineNumber);

        TokenType getTokenType();

        std::string getLexeme();

        [[nodiscard]] int getContextPosition() const;

        [[nodiscard]] int getLineNumber() const;

        static bool isSingleCharacterToken(std::string &token);

        static bool isKeyWord(std::string &token);

        static std::unordered_map<std::string, TokenType> singleCharacterTokens;
        static std::unordered_map<std::string, TokenType> keywords;
    };

} // CoolCompiler

#endif //COOLCOMPILER_TOKEN_H
