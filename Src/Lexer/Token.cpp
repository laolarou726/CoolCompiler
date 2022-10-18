//
// Created by luoli on 2022/9/14.
//

#include <algorithm>
#include "Token.h"
#include "../Utils/StringUtils.h"

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

    std::unordered_map<char, char> Token::specialCharacters = {
            {'a', '\a'},
            {'b', '\b'},
            {'f', '\f'},
            {'n', '\n'},
            {'r', '\r'},
            {'t', '\t'},
            {'v', '\v'},
            {'\\', '\\'},
            {'\'', '\''},
            {'"', '\"'},
            {'?', '\?'}
    };

    Token::Token() {
        this->tokenType = E0F;
        this->contextPosition = -1;
        this->lineNumber = -1;
    }

    Token::Token(TokenType tokenType, const std::string &lexeme, int contextPosition, int lineNumber) {
        this->tokenType = tokenType;
        this->lexeme = lexeme;
        this->contextPosition = contextPosition;
        this->lineNumber = lineNumber;
    }

    TokenType Token::getTokenType() {
        return tokenType;
    }

    std::string Token::getLexeme() const {
        return lexeme;
    }

    int Token::getContextPosition() const {
        return contextPosition;
    }

    int Token::getLineNumber() const {
        return lineNumber;
    }

    bool Token::isSpecialCharacter(const char &token) {
        return specialCharacters.find(token) != specialCharacters.end();
    }

    bool Token::isSingleCharacterToken(const std::string &token) {
        return singleCharacterTokens.find(token) != singleCharacterTokens.end();
    }

    bool Token::isKeyWord(const std::string &token) {
        return keywords.find(token) != keywords.end();
    }

    std::string Token::recoverCharTrans(const std::string &value) {
        std::string val = value;

        for(auto p : specialCharacters){
            std::string from, to = "\\";
            from.push_back(p.second);
            to.push_back(p.first);

            StringUtils::replaceAll(val, from, to);
        }

        return val;
    }

} // CoolCompiler