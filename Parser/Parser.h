//
// Created by luoli on 2022/9/19.
//

#ifndef COOLCOMPILER_PARSER_H
#define COOLCOMPILER_PARSER_H

#include <vector>
#include "../Lexer/Token.h"
#include "AST/AST.h"

namespace CoolCompiler {

    class Parser {
    private:
        std::vector<Token> tokens;
        int position;
        Token currentToken;
        AST parseTree;

        void Fail(const std::string &errorMessage);
        Token expect(TokenType tokenType, const std::string &errorMessage = "");
        Token next(int step = 1);
        bool hasNext();
        [[nodiscard]] Token peek(int lookAhead = 1) const;
    public:
        explicit Parser(const std::vector<Token> &tokens);
        void parse();
        [[nodiscard]] AST getParseTree() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_PARSER_H
