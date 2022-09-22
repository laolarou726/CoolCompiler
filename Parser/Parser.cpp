//
// Created by luoli on 2022/9/19.
//

#include <iostream>
#include <sstream>
#include "Parser.h"
// AST Structures
#include "AST/Expression/Expression.h"
#include "AST/FeatureAttribute.h"
#include "AST/FeatureMethod.h"
#include "AST/Expression/Assignment.h"
// Expressions
#include "AST/Expression/If.h"
#include "AST/Expression/While.h"
#include "AST/Expression/Block.h"
#include "AST/Expression/Let.h"
#include "AST/Expression/InnerLet.h"
#include "AST/Expression/Case.h"
#include "AST/Expression/CaseAction.h"
#include "AST/Expression/New.h"
#include "AST/Expression/IsVoid.h"
#include "AST/Expression/Tilde.h"
#include "AST/Expression/MathBinop.h"
#include "AST/Expression/Not.h"
#include "AST/Expression/Id.h"
#include "AST/Expression/Integer.h"
#include "AST/Expression/String.h"
#include "AST/Expression/Boolean.h"

namespace CoolCompiler {
    Parser::Parser(const std::vector<Token> &tokens) {
        this->tokens = tokens;
        this->position = -1;
    }

    void Parser::parse() {
        PROGRAM(this->parseTree);
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

    // AST Resolver

    void Parser::PROGRAM(std::vector<AST> &container) {
        CLASS(container);
        expect(SEMICOLON);

        while(hasNext()){
            CLASS(container);
            expect(SEMICOLON);
        }
    }

    void Parser::CLASS(std::vector<AST> &container) {
        expect(TokenType::CLASS);
        Token typeId = expect(TYPE_ID);

        Token inheritType;
        if(peek().getTokenType() == INHERITS){
            next();
            inheritType = expect(TYPE_ID);
        }
        expect(LEFT_BRACE);

        std::vector<AST> features;
        while(peek().getTokenType() != RIGHT_BRACE && peek(2).getTokenType() != SEMICOLON){
            FEATURE(features);
            expect(SEMICOLON);
        }
        expect(RIGHT_BRACE);

        container.emplace_back(Class(typeId.getLexeme(), features, inheritType.getLexeme()));
    }

    void Parser::FEATURE_ATTR(const Token &objId, std::vector<AST> &container) {
        Token typeId = expect(TYPE_ID);
        std::vector<Expression> expressions;

        if(peek().getTokenType() == ASSIGN){
            next();
            EXPRESSION(expressions);
        }

        container.emplace_back(FeatureAttribute(objId.getLexeme(), typeId.getLexeme(), expressions.front()));
    }

    void Parser::FEATURE_METHOD(const Token &objId, std::vector<AST> &container) {
        std::vector<Formal> formals;

        if(peek().getTokenType() != RIGHT_PAREN){
            FORMAL(formals);

            while(peek().getTokenType() != RIGHT_PAREN){
                expect(COMMA);
                FORMAL(formals);
            }
        }

        expect(RIGHT_PAREN);
        expect(COLON);

        Token typeId = expect(TYPE_ID);

        expect(LEFT_BRACE);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        expect(RIGHT_BRACE);

        container.emplace_back(
                FeatureMethod(objId.getLexeme(),
                              typeId.getLexeme(),
                              expressions.front(),
                              formals));
    }

    void Parser::FEATURE(std::vector<AST> &container) {
        Token objId = expect(OBJ_ID);

        if(peek().getTokenType() == LEFT_PAREN){
            next();
            FEATURE_METHOD(objId, container);
        }

        if(peek().getTokenType() == COLON){
            next();
            FEATURE_ATTR(objId, container);
        }
    }

    void Parser::FORMAL(std::vector<Formal> &container) {
        Token objId = expect(OBJ_ID);

        expect(COLON);

        Token typeId = expect(TYPE_ID);

        container.emplace_back(Formal(objId.getLexeme(), typeId.getLexeme()));
    }

    // Expressions

    void Parser::ASSIGNMENT(std::vector<Expression> &container) {
        Token objId = expect(OBJ_ID);

        expect(ASSIGN);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        container.emplace_back(Assignment(Id(objId.getLexeme()), expressions.front()));
    }

    void Parser::METHOD_ACCESS(std::vector<Expression> &container) {

    }

    void Parser::IF(std::vector<Expression> &container) {
        expect(TokenType::IF);

        std::vector<Expression> conditionExpression;
        EXPRESSION(conditionExpression);

        expect(THEN);

        std::vector<Expression> trueExpression;
        EXPRESSION(trueExpression);

        expect(ELSE);

        std::vector<Expression> falseExpression;
        EXPRESSION(falseExpression);

        expect(FI);

        container.emplace_back(
                If(conditionExpression.front(),
                   trueExpression.front(),
                   falseExpression.front()));
    }

    void Parser::WHILE(std::vector<Expression> &container) {
        expect(TokenType::WHILE);

        std::vector<Expression> conditionExpression;
        EXPRESSION(conditionExpression);

        expect(LOOP);

        std::vector<Expression> bodyExpression;
        EXPRESSION(bodyExpression);

        expect(POOL);

        container.emplace_back(While(conditionExpression.front(), bodyExpression.front()));
    }

    void Parser::BLOCK(std::vector<Expression> &container) {
        expect(LEFT_BRACE);

        std::vector<Expression> expressions;

        std::vector<Expression> expression1;
        EXPRESSION(expression1);

        expressions.emplace_back(expression1.front());

        expect(SEMICOLON);

        while (peek().getTokenType() != RIGHT_BRACE){
            std::vector<Expression> expression2;
            EXPRESSION(expression2);

            expressions.emplace_back(expression2.front());

            expect(SEMICOLON);
        }

        expect(RIGHT_BRACE);

        container.emplace_back(Block(expressions));
    }

    void Parser::LET(std::vector<Expression> &container) {
        expect(TokenType::LET);

        Token objId = expect(OBJ_ID);

        expect(COLON);

        Token typeId = expect(TYPE_ID);

        std::vector<InnerLet> innerLets;
        if(peek().getTokenType() == ASSIGN){
            expect(ASSIGN);

            std::vector<Expression> assignExpressions;
            EXPRESSION(assignExpressions);

            innerLets.emplace_back(
                    InnerLet(objId.getLexeme(),
                             typeId.getLexeme(),
                             assignExpressions.front()));
        }

        if(peek().getTokenType() == COMMA){
            while(peek().getTokenType() != IN){
                expect(COMMA);

                Token objId1 = expect(OBJ_ID);

                expect(COLON);

                Token typeId1 = expect(TYPE_ID);

                if(peek().getTokenType() == ASSIGN){
                    expect(ASSIGN);

                    std::vector<Expression> assignExpressions1;
                    EXPRESSION(assignExpressions1);

                    innerLets.emplace_back(
                            InnerLet(objId1.getLexeme(),
                                     typeId1.getLexeme(),
                                     assignExpressions1.front()));
                }
            }
        }

        expect(IN);

        std::vector<Expression> inExpressions;
        EXPRESSION(inExpressions);

        container.emplace_back(Let(innerLets, inExpressions.front()));
    }

    void Parser::CASE(std::vector<Expression> &container) {
        expect(TokenType::CASE);

        std::vector<Expression> caseExpressions;
        EXPRESSION(caseExpressions);

        expect(OF);

        Token objId1 = expect(TokenType::OBJ_ID);

        expect(COLON);

        Token typeId1 = expect(TokenType::TYPE_ID);

        expect(GTOE);

        std::vector<Expression> caseAction1;
        EXPRESSION(caseAction1);

        expect(SEMICOLON);

        std::vector<CaseAction> actions;
        actions.emplace_back(CaseAction(objId1.getLexeme(), typeId1.getLexeme(), caseAction1.front()));

        if(peek().getTokenType() == OBJ_ID){
            while (peek().getTokenType() != ESAC){
                Token objIdN = expect(TokenType::OBJ_ID);

                expect(COLON);

                Token typeIdN = expect(TokenType::TYPE_ID);

                expect(GTOE);

                std::vector<Expression> caseActionN;
                EXPRESSION(caseAction1);

                expect(SEMICOLON);

                actions.emplace_back(CaseAction(objIdN.getLexeme(), objIdN.getLexeme(), caseActionN.front()));
            }
        }

        expect(ESAC);

        container.emplace_back(Case(caseExpressions.front(), actions));
    }

    void Parser::NEW(std::vector<Expression> &container) {
        expect(TokenType::NEW);

        Token typeId = expect(TYPE_ID);

        container.emplace_back(New(typeId.getLexeme()));
    }

    void Parser::IS_VOID(std::vector<Expression> &container) {
        expect(TokenType::ISVOID);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        container.emplace_back(IsVoid(expressions.front()));
    }

    void Parser::PLUS(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::PLUS, left, container);
    }

    void Parser::MINUS(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::MINUS, left, container);
    }

    void Parser::STAR(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::STAR, left, container);
    }

    void Parser::SLASH(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::SLASH, left, container);
    }

    void Parser::TILDE(std::vector<Expression> &container) {
        expect(TokenType::TILDE);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        container.emplace_back(Tilde(expressions.front()));
    }

    void Parser::LESS_THAN(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::LT, left, container);
    }

    void Parser::LESS_THAN_EQ(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::LTOE, left, container);
    }

    void Parser::EQ(const Expression &left, std::vector<Expression> &container) {
        MATH_BINOP(TokenType::EQ, left, container);
    }

    void Parser::MATH_BINOP(TokenType tokenType, const Expression &left, std::vector<Expression> &container) {
        expect(tokenType);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        Expression right = expressions.front();

        container.emplace_back(MathBinop(tokenType, left, right));
    }

    void Parser::NOT(std::vector<Expression> &container) {
        expect(TokenType::NOT);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        container.emplace_back(Not(expressions.front()));
    }

    void Parser::PAREN(std::vector<Expression> &container) {
        expect(LEFT_PAREN);

        std::vector<Expression> expressions;
        EXPRESSION(expressions);

        expect(RIGHT_PAREN);

        container.emplace_back(expressions.front());
    }

    void Parser::ID(std::vector<Expression> &container) {
        Token idToken = expect(TokenType::OBJ_ID);

        container.emplace_back(Id(idToken.getLexeme()));
    }

    void Parser::INTEGER(std::vector<Expression> &container) {
        Token numToken = expect(TokenType::NUMBER);
        int number = std::stoi(numToken.getLexeme());

        container.emplace_back(Integer(number));
    }

    void Parser::STRING(std::vector<Expression> &container) {
        Token strToken = expect(TokenType::STRING);

        container.emplace_back(String(strToken.getLexeme()));
    }

    void Parser::TRUE(std::vector<Expression> &container) {
        expect(TokenType::TRUE);
        container.emplace_back(Boolean(true));
    }

    void Parser::FALSE(std::vector<Expression> &container) {
        expect(TokenType::FALSE);
        container.emplace_back(Boolean(false));
    }

    void Parser::EXPRESSION(std::vector<Expression> &container) {
        switch (peek().getTokenType()) {
            case TokenType::OBJ_ID:
                break;
            case TokenType::IF:
                IF(container);
                break;
            case TokenType::WHILE:
                WHILE(container);
                break;
            case TokenType::LEFT_BRACE:
                BLOCK(container);
                break;
            case TokenType::LET:
                LET(container);
                break;
            case TokenType::CASE:
                CASE(container);
                break;
            case TokenType::NEW:
                NEW(container);
                break;
            case TokenType::ISVOID:
                IS_VOID(container);
                break;
            case TokenType::TILDE:
                TILDE(container);
                break;
            case TokenType::NOT:
                NOT(container);
                break;
            case TokenType::LEFT_PAREN:
                PAREN(container);
                break;
            case TokenType::NUMBER:
                INTEGER(container);
                break;
            case TokenType::STRING:
                STRING(container);
                break;
            case TokenType::TRUE:
                TRUE(container);
                break;
            case TokenType::FALSE:
                FALSE(container);
                break;
            default:
                std::vector<Expression> leftExpressions;
                EXPRESSION(leftExpressions);

                Expression left = leftExpressions.front();

                switch (peek().getTokenType()) {
                    case TokenType::PLUS:
                        PLUS(left, container);
                        break;
                    case TokenType::MINUS:
                        MINUS(left, container);
                        break;
                    case TokenType::STAR:
                        STAR(left, container);
                        break;
                    case TokenType::SLASH:
                        SLASH(left, container);
                        break;
                    case TokenType::LT:
                        LESS_THAN(left, container);
                        break;
                    case TokenType::LTOE:
                        LESS_THAN_EQ(left, container);
                        break;
                    case TokenType::EQ:
                        EQ(left, container);
                        break;
                }
        }
    }

    // End of Expressions
    // End of AST Resolver
} // CoolCompiler