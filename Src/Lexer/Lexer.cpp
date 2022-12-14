//
// Created by luoli on 2022/9/14.
//

#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "../Utils/StringUtils.h"

namespace CoolCompiler {
    Lexer::Lexer(const std::string &filePath) {
        std::ifstream t(filePath);
        std::stringstream buffer;

        buffer << t.rdbuf();
        source = buffer.str();

        sourcePosition = 0;
        contextPosition = 1;
        lineNumber = 1;
    }

    std::string Lexer::getSource() {
        return source;
    }

    bool Lexer::skipBlank() {
        while (isspace(source[sourcePosition])) {
            if (source[sourcePosition] == '\n') {
                lineNumber++;
            }

            contextPosition++;
            sourcePosition++;

            if (sourcePosition + 1 > source.length())
                return true;
        }

        return false;
    }

    bool Lexer::isLiteral() {
        int pos = sourcePosition;
        std::string lexeme;
        std::string comment;

        isComment(pos, comment);
        isOneLineComment(pos, comment);

        return isTypeId(pos, lexeme) ||
               isObjId(pos, lexeme) ||
               isNumber(pos, lexeme) ||
               isString(pos, lexeme);
    }

    bool Lexer::isString(int &pos, std::string &lexeme) {
        if (source[pos] == '"') {
            pos += 1;

            bool isSpecialChar = false;
            while (source[pos] != '"') {
                if(isSpecialChar){
                    if(Token::isSpecialCharacter(source[pos + 1])){
                        lexeme += Token::specialCharacters[source[pos + 1]];
                    }

                    pos += 2;
                    contextPosition++;
                    isSpecialChar = false;
                    continue;
                }

                if (source[pos] == '\\')
                {
                    isSpecialChar = true;
                    continue;
                }

                moveNext(pos, lexeme);

                if (pos + 1 > source.length())
                    break;
            }

            addToken(TokenType::STRING, lexeme);
            sourcePosition = pos + 1;

            return true;
        }

        return false;
    }

    bool Lexer::isNumber(int &pos, std::string &lexeme) {
        if (isdigit(source[pos])) {
            while (isdigit(source[pos])) {
                moveNext(pos, lexeme);

                if (pos + 1 > source.length())
                    break;
            }

            addToken(TokenType::NUMBER, lexeme);
            sourcePosition = pos;

            return true;
        }

        return false;
    }

    bool Lexer::isTypeId(int &pos, std::string &lexeme) {
        if (isalpha(source[pos]) && isupper(source[pos])) {
            while (isalnum(source[pos]) || source[pos] == '_') {
                moveNext(pos, lexeme);

                if (pos + 1 > source.length())
                    break;
            }

            if (isSelfType(lexeme))
                addToken(TokenType::SELF_TYPE, lexeme);
            else
                addToken(TokenType::TYPE_ID, lexeme);

            sourcePosition = pos;

            return true;
        }

        return false;
    }

    bool Lexer::isObjId(int &pos, std::string &lexeme) {
        if (isalpha(source[pos])) {
            while (isalnum(source[pos]) || source[pos] == '_') {
                moveNext(pos, lexeme);

                if (pos + 1 > source.length())
                    break;
            }

            std::string lowerLexeme = StringUtils::toLower(lexeme);

            if (isSelf(lowerLexeme))
                addToken(TokenType::SELF, lowerLexeme);
            else if (isKeyword(lowerLexeme))
                addToken(Token::keywords[lowerLexeme], lowerLexeme);
            else
                addToken(TokenType::OBJ_ID, lexeme);

            sourcePosition = pos;

            return true;
        }

        return false;
    }

    bool Lexer::isSelf(std::string &lexeme) {
        return lexeme == "self";
    }

    bool Lexer::isKeyword(std::string &lexeme) {
        return Token::isKeyWord(lexeme);
    }

    bool Lexer::isSelfType(std::string &lexeme) {
        return lexeme == "SELF_TYPE";
    }

    void Lexer::moveNext(int &pos, std::string &lexeme) {
        lexeme += source[pos];
        pos++;
        contextPosition++;
    }

    void Lexer::addToken(const TokenType &tokenType, std::string &lexeme) {
        tokens.emplace_back(tokenType,
                            lexeme,
                            contextPosition - lexeme.length(),
                            lineNumber);
    }

    bool Lexer::isSingleCharacterToken() {
        std::string ch;
        ch.push_back(source[sourcePosition]);

        if (!Token::isSingleCharacterToken(ch)) return false;

        TokenType tokenType = Token::singleCharacterTokens[ch];

        if (tokenType == LT || tokenType == GT && sourcePosition < source.length()) {
            char nextCh = source[sourcePosition + 1];

            if (nextCh == '=' || nextCh == '-') {
                sourcePosition++;
                contextPosition++;

                if (tokenType == GT && nextCh == '=') {
                    tokenType = GTOE;
                    ch.push_back(nextCh);
                }

                if (tokenType == LT) {
                    if (nextCh == '=')
                        tokenType = LTOE;
                    if (nextCh == '-')
                        tokenType = ASSIGN;

                    ch.push_back(nextCh);
                }
            }
        }

        int ctxPosition = (tokenType == LTOE || tokenType == GTOE || tokenType == ASSIGN)
                          ? contextPosition - 1
                          : contextPosition;
        Token token = Token(tokenType, ch, ctxPosition, lineNumber);

        tokens.emplace_back(token);
        sourcePosition++;
        contextPosition++;

        return true;
    }

    bool Lexer::isComment(int &pos, std::string &lexeme) {
        std::string ch;
        ch.push_back(source[sourcePosition]);

        if (!Token::isSingleCharacterToken(ch)) return false;

        TokenType tokenType = Token::singleCharacterTokens[ch];

        if(tokenType != LEFT_PAREN) return false;

        char nextCh = source[sourcePosition + 1];

        if(nextCh != '*') return false;

        moveNext(pos, lexeme);
        moveNext(pos, lexeme);

        while (pos + 1 < source.length() && !skipBlank()) {
            moveNext(pos, lexeme);

            if (source[pos] == '*' && source[pos + 1] == ')')
                break;
        }

        moveNext(pos, lexeme);
        moveNext(pos, lexeme);

        sourcePosition = pos;

        return true;
    }

    bool Lexer::isOneLineComment(int &pos, std::string &lexeme) {
        std::string ch;
        ch.push_back(source[sourcePosition]);

        if (!Token::isSingleCharacterToken(ch)) return false;

        TokenType tokenType = Token::singleCharacterTokens[ch];

        if(tokenType != MINUS) return false;

        char nextCh = source[sourcePosition + 1];

        if(nextCh != '-') return false;

        moveNext(pos, lexeme);
        moveNext(pos, lexeme);

        while ((pos + 1 < source.length() || isspace(source[pos])) && source[pos] != '\n') {
            moveNext(pos, lexeme);

            if (source[pos] == '\n')
                break;
        }

        sourcePosition = pos;

        return true;
    }

    void Lexer::doScan() {
        while (sourcePosition < source.length()) {
            if (skipBlank()) break;
            if (!(isLiteral() || isSingleCharacterToken()) && !isspace(source[sourcePosition])) {
                std::ostringstream messageStream;
                messageStream
                        << "Syntax error -> " << lineNumber
                        << ":" << contextPosition
                        << " | unknown symbol: " << source[sourcePosition];

                Error err = Error(contextPosition, lineNumber, messageStream.str());

                errors.emplace_back(err);
                sourcePosition++;
                contextPosition++;
            }
        }

        Token EOFToken = Token(E0F, "$", 0, lineNumber + 1);
        tokens.emplace_back(EOFToken);
    }

    std::vector<Token> Lexer::getTokens() const {
        return tokens;
    }

    std::vector<Error> Lexer::getErrors() const {
        return errors;
    }

} // CoolCompiler