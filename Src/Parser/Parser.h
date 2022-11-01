//
// Created by luoli on 2022/9/19.
//

#ifndef COOLCOMPILER_PARSER_H
#define COOLCOMPILER_PARSER_H

#include <vector>
#include "AST/AST.h"
#include "AST/Class.h"
#include "AST/Expression/Expression.h"
#include "AST/Formal.h"
#include "AST/Program.h"
#include "../Lexer/Error.h"
#include "../Lexer/Token.h"

namespace CoolCompiler {

    class Parser {
    private:
        std::vector<Token> tokens;
        std::vector<Error> errors;

        int position;
        Token currentToken;
        Program* parseTree;

        void fail(const std::string &errorMessage);
        Token expect(TokenType tokenType, const std::string &errorMessage = "");
        Token next(int step = 1);
        bool hasNext();
        [[nodiscard]] Token peek(int lookAhead = 1) const;

        // AST Resolver

        void PROGRAM();
        void CLASS(std::vector<Class*> &container);
        void FEATURE_ATTR(const Token &objId, std::vector<FeatureBase*> &container);
        void FEATURE_METHOD(const Token &objId, std::vector<FeatureBase*> &container);
        void FEATURE(std::vector<FeatureBase*> &container);
        void FORMAL(std::vector<Formal*> &container);

        // Expressions

        void ASSIGNMENT(std::vector<Expression*> &container);
        void IF(std::vector<Expression*> &container);
        void WHILE(std::vector<Expression*> &container);
        void BLOCK(std::vector<Expression*> &container);
        void METHOD_ACCESS(Expression* left, std::vector<Expression*> &container);
        void AT_METHOD_ACCESS(Expression* left, std::vector<Expression*> &container);
        void SELF_METHOD_ACCESS(const std::string &method, std::vector<Expression*> &container);
        void LET(std::vector<Expression*> &container);
        void CASE(std::vector<Expression*> &container);
        void NEW(std::vector<Expression*> &container);
        void IS_VOID(std::vector<Expression*> &container);

        // MATH OPS
        void PLUS(Expression* left, std::vector<Expression*> &container);
        void MINUS(Expression* left, std::vector<Expression*> &container);
        void STAR(Expression* left, std::vector<Expression*> &container);
        void SLASH(Expression* left, std::vector<Expression*> &container);
        void MATH_BINOP(TokenType tokenType, Expression* left, std::vector<Expression*> &container);
        void MATH_OPS(Expression* left, std::vector<Expression*> &container);

        // COMPARISON
        void TILDE(std::vector<Expression*> &container);
        void LESS_THAN(Expression* left, std::vector<Expression*> &container);
        void LESS_THAN_EQ(Expression* left, std::vector<Expression*> &container);
        void GREATER_THAN(Expression* left, std::vector<Expression*> &container);
        void GREATER_THAN_EQ(Expression* left, std::vector<Expression*> &container);
        void EQ(Expression* left, std::vector<Expression*> &container);
        void COMPARISON(TokenType tokenType, Expression* left, std::vector<Expression*> &container);
        void COMPARISON_OPS(Expression* left, std::vector<Expression*> &container);

        void NOT(std::vector<Expression*> &container);
        void PAREN(std::vector<Expression*> &container);
        void ID(std::vector<Expression*> &container);
        void INTEGER(std::vector<Expression*> &container);
        void STRING(std::vector<Expression*> &container);
        void TRUE(std::vector<Expression*> &container);
        void FALSE(std::vector<Expression*> &container);
        void EXPRESSION(std::vector<Expression*> &container);

        // End of Expressions

        // End of AST Resolver
    public:
        explicit Parser(const std::vector<Token> &tokens);
        void parse();
        [[nodiscard]] Program* getParseTree() const;
        [[nodiscard]] std::vector<Error> getErrors() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_PARSER_H
