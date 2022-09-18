//
// Created by luoli on 2022/9/14.
//

#include "Token.h"

namespace CoolCompiler {

    std::unordered_map<std::string, TokenType> Token::singleCharacterTokens = {
            {"(",  TokenType::LEFT_PAREN},
            {")",  TokenType::RIGHT_PAREN},
            {"{",  TokenType::LEFT_BRACE},
            {"}",  TokenType::RIGHT_BRACE},
            {",",  TokenType::COMMA},
            {".",  TokenType::DOT},
            {"-",  TokenType::MINUS},
            {"+",  TokenType::PLUS},
            {":",  TokenType::COLON},
            {";",  TokenType::SEMICOLON},
            {"/",  TokenType::SLASH},
            {"*",  TokenType::STAR},
            {"=",  TokenType::EQ},
            {"<",  TokenType::LT},
            {">",  TokenType::GT},
            {"<=", TokenType::LTOE},
            {">=", TokenType::GTOE},
            {"<-", TokenType::ASSIGN},
            {"~",  TokenType::TILDE},
            {"@",  TokenType::AT}
    };

    std::unordered_map<std::string, TokenType> Token::keywords = {
            {"class",    TokenType::CLASS},
            {"else",     TokenType::ELSE},
            {"false",    TokenType::FALSE},
            {"fi",       TokenType::FI},
            {"if",       TokenType::IF},
            {"in",       TokenType::IN},
            {"inherits", TokenType::INHERITS},
            {"isvoid",   TokenType::ISVOID},
            {"let",      TokenType::LET},
            {"loop",     TokenType::LOOP},
            {"pool",     TokenType::POOL},
            {"then",     TokenType::THEN},
            {"while",    TokenType::WHILE},
            {"case",     TokenType::CASE},
            {"esac",     TokenType::ESAC},
            {"new",      TokenType::NEW},
            {"of",       TokenType::OF},
            {"not",      TokenType::NOT},
            {"true",     TokenType::TRUE}
    };

    Token::Token(TokenType tokenType, const std::string &lexeme, int contextPosition, int lineNumber) {
        this->tokenType = tokenType;
        this->lexeme = lexeme;
        this->contextPosition = contextPosition;
        this->lineNumber = lineNumber;
    }

    TokenType Token::getTokenType() {
        return tokenType;
    }

    std::string Token::getLexeme() {
        return lexeme;
    }

    int Token::getContextPosition() const {
        return contextPosition;
    }

    int Token::getLineNumber() const {
        return lineNumber;
    }

    bool Token::isSingleCharacterToken(std::string &token) {
        return singleCharacterTokens.find(token) != singleCharacterTokens.end();
    }

    bool Token::isKeyWord(std::string &token) {
        return keywords.find(token) != keywords.end();
    }
} // CoolCompiler