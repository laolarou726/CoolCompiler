//
// Created by luoli on 2022/9/19.
//

#include <iostream>
#include <sstream>
#include "Parser.h"

namespace CoolCompiler {
    Parser::Parser(const std::vector<Token> &tokens) {
        this->tokens = tokens;
        this->position = -1;
    }

    void Parser::parse() {
    }

    void Parser::Fail(const std::string &errorMessage = "") {
        std::string err = errorMessage.empty() ? "Syntax error" : errorMessage;
        std::cout
            << "Syntax error : " << errorMessage << std::endl
            << "Line         :"  << currentToken.getLineNumber() << std::endl
            << "Column       :"  << currentToken.getContextPosition() << std::endl;
    }

    Token Parser::expect(TokenType tokenType, const std::string &errorMessage) {
        Token nextToken = next();

        if(nextToken.getTokenType() != tokenType){
            std::ostringstream messageStream;
            messageStream << "Expect " << tokenType << " but get " << nextToken.getTokenType();

            Fail(messageStream.str());
            return {};
        }

        return nextToken;
    }

    Token Parser::next(int step) {
        position += step;
        currentToken = tokens[position];
        return currentToken;
    }

    bool Parser::hasNext() {
        return peek().getTokenType() != E0F;
    }

    Token Parser::peek(int lookAhead) const {
        return tokens[position + lookAhead];
    }

    std::vector<AST> Parser::getParseTree() const {
        return parseTree;
    }
} // CoolCompiler