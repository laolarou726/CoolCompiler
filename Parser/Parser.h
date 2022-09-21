//
// Created by luoli on 2022/9/19.
//

#ifndef COOLCOMPILER_PARSER_H
#define COOLCOMPILER_PARSER_H

#include <vector>
#include "../Lexer/Token.h"
#include "AST/AST.h"
#include "AST/Class.h"

namespace CoolCompiler {

    class Parser {
    private:
        std::vector<Token> tokens;
        int position;
        Token currentToken;
        std::vector<AST> parseTree;

        void Fail(const std::string &errorMessage);
        Token expect(TokenType tokenType, const std::string &errorMessage = "");
        Token next(int step = 1);
        bool hasNext();
        [[nodiscard]] Token peek(int lookAhead = 1) const;

        // AST Resolver

        void PROGRAM(std::vector<AST> &container);
        void CLASS(std::vector<AST> &container);
        void FEATURE_ATTR(std::vector<AST> &container);
        void FEATURE_METHOD(std::vector<AST> &container);
        void FEATURE(std::vector<AST> &container);
        void FORMAL(std::vector<AST> &container);
        void ASSIGNMENT(std::vector<AST> &container);


        // End of AST Resolver
    public:
        explicit Parser(const std::vector<Token> &tokens);
        void parse();
        [[nodiscard]] std::vector<AST> getParseTree() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_PARSER_H
