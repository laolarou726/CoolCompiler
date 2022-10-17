//
// Created by luoli on 2022/9/19.
//

#include <iostream>
#include <sstream>
#include "Parser.h"
// AST Structures
#include "AST/AST.h"
#include "AST/Program.h"
#include "AST/Expression/Expression.h"
#include "AST/FeatureAttribute.h"
#include "AST/FeatureMethod.h"
#include "AST/Expression/Assignment.h"
// Expressions
#include "AST/Expression/Self.h"
#include "AST/Expression/If.h"
#include "AST/Expression/While.h"
#include "AST/Expression/Block.h"
#include "AST/Expression/MethodAccess.h"
#include "AST/Expression/SelfMethodAccess.h"
#include "AST/Expression/AtMethodAccess.h"
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
        this->parseTree = new Program();
        PROGRAM();
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

    Program* Parser::getParseTree() const {
        return parseTree;
    }

    // AST Resolver

    void Parser::PROGRAM() {
        CLASS(*this->parseTree->getClasses());
        expect(SEMICOLON);

        while(hasNext()){
            CLASS(*this->parseTree->getClasses());
            expect(SEMICOLON);
        }
    }

    void Parser::CLASS(std::vector<AST*> &container) {
        expect(TokenType::CLASS);
        Token typeId = expect(TYPE_ID);

        Token inheritType;
        if(peek().getTokenType() == INHERITS){
            next();
            inheritType = expect(TYPE_ID);
        }
        expect(LEFT_BRACE);

        std::vector<AST*> features;
        while(peek().getTokenType() != RIGHT_BRACE && peek(2).getTokenType() != SEMICOLON){
            FEATURE(features);
            expect(SEMICOLON);
        }
        expect(RIGHT_BRACE);

        container.emplace_back(new Class(typeId.getLexeme(), features, inheritType.getLexeme()));
    }

    void Parser::FEATURE_ATTR(const Token &objId, std::vector<AST*> &container) {
        Token typeId = expect(TYPE_ID);
        std::vector<Expression*> expressions;

        if(peek().getTokenType() == ASSIGN){
            next();
            EXPRESSION(expressions);
            container.emplace_back(new FeatureAttribute(objId.getLexeme(), typeId.getLexeme(), expressions.front()));
            return;
        }

        container.emplace_back(new FeatureAttribute(objId.getLexeme(), typeId.getLexeme()));
    }

    void Parser::FEATURE_METHOD(const Token &objId, std::vector<AST*> &container) {
        std::vector<Formal*> formals;

        if(peek().getTokenType() != RIGHT_PAREN){
            FORMAL(formals);

            while(peek().getTokenType() != RIGHT_PAREN){
                expect(COMMA);
                FORMAL(formals);
            }
        }

        expect(RIGHT_PAREN);
        expect(COLON);

        Token typeId;
        if(peek().getTokenType() == SELF_TYPE){
            typeId = expect(SELF_TYPE);
        }
        else{
            typeId = expect(TYPE_ID);
        }

        expect(LEFT_BRACE);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        expect(RIGHT_BRACE);

        container.emplace_back(
                new FeatureMethod(objId.getLexeme(),
                              typeId.getLexeme(),
                              expressions.front(),
                              formals));
    }

    void Parser::FEATURE(std::vector<AST*> &container) {
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

    void Parser::FORMAL(std::vector<Formal*> &container) {
        Token objId = expect(OBJ_ID);

        expect(COLON);

        Token typeId = expect(TYPE_ID);

        container.emplace_back(new Formal(objId.getLexeme(), typeId.getLexeme()));
    }

    // Expressions

    void Parser::ASSIGNMENT(std::vector<Expression*> &container) {
        Token objId = expect(OBJ_ID);

        expect(ASSIGN);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        container.emplace_back(new Assignment(new Id(objId.getLexeme()), expressions.front()));
    }

    void Parser::IF(std::vector<Expression*> &container) {
        expect(TokenType::IF);

        std::vector<Expression*> conditionExpression;
        EXPRESSION(conditionExpression);

        expect(THEN);

        std::vector<Expression*> trueExpression;
        EXPRESSION(trueExpression);

        expect(ELSE);

        std::vector<Expression*> falseExpression;
        EXPRESSION(falseExpression);

        expect(FI);

        container.emplace_back(
                new If(conditionExpression.front(),
                   trueExpression.front(),
                   falseExpression.front()));
    }

    void Parser::WHILE(std::vector<Expression*> &container) {
        expect(TokenType::WHILE);

        std::vector<Expression*> conditionExpression;
        EXPRESSION(conditionExpression);

        expect(LOOP);

        std::vector<Expression*> bodyExpression;
        EXPRESSION(bodyExpression);

        expect(POOL);

        container.emplace_back(new While(conditionExpression.front(), bodyExpression.front()));
    }

    void Parser::BLOCK(std::vector<Expression*> &container) {
        expect(LEFT_BRACE);

        std::vector<Expression*> expressions;

        EXPRESSION(expressions);

        expect(SEMICOLON);

        while (peek().getTokenType() != RIGHT_BRACE){
            EXPRESSION(expressions);

            expect(SEMICOLON);
        }

        expect(RIGHT_BRACE);

        container.emplace_back(new Block(expressions));
    }

    void Parser::METHOD_ACCESS(Expression* left, std::vector<Expression*> &container) {
        Token methodToken = expect(OBJ_ID);

        expect(LEFT_PAREN);

        std::vector<Expression*> parameters;

        if(peek().getTokenType() != RIGHT_PAREN){
            EXPRESSION(parameters);

            while(peek().getTokenType() != RIGHT_PAREN){
                expect(COMMA);
                EXPRESSION(parameters);
            }
        }

        expect(RIGHT_PAREN);

        Expression* result = new MethodAccess(left, methodToken.getLexeme(), parameters);
        std::vector<Expression*> resultExpr;

        if(peek().getTokenType() == DOT){
            next();
            METHOD_ACCESS(result, resultExpr);
            container.emplace_back(resultExpr.back());
            return;
        }

        if(peek().getTokenType() == AT){
            next();
            AT_METHOD_ACCESS(result, resultExpr);
            container.emplace_back(resultExpr.back());
            return;
        }

        container.emplace_back(result);
    }

    void Parser::AT_METHOD_ACCESS(Expression* left, std::vector<Expression*> &container) {
        Token typeToken = expect(TYPE_ID);

        expect(DOT);

        Token methodToken = expect(OBJ_ID);

        expect(LEFT_PAREN);

        std::vector<Expression*> parameters;

        if(peek().getTokenType() != RIGHT_PAREN){
            EXPRESSION(parameters);

            while(peek().getTokenType() != RIGHT_PAREN){
                expect(COMMA);
                EXPRESSION(parameters);
            }
        }

        expect(RIGHT_PAREN);

        Expression* result = new AtMethodAccess(left,
                                                typeToken.getLexeme(),
                                                methodToken.getLexeme(),
                                                parameters);
        std::vector<Expression*> resultExpr;

        if(peek().getTokenType() == DOT){
            next();
            METHOD_ACCESS(result, resultExpr);
            container.emplace_back(resultExpr.back());
            return;
        }

        if(peek().getTokenType() == AT){
            next();
            AT_METHOD_ACCESS(result, resultExpr);
            container.emplace_back(resultExpr.back());
            return;
        }

        container.emplace_back(result);
    }

    void Parser::SELF_METHOD_ACCESS(const std::string &method, std::vector<Expression*> &container) {
        expect(LEFT_PAREN);

        std::vector<Expression*> parameters;

        if(peek().getTokenType() != RIGHT_PAREN){
            EXPRESSION(parameters);

            while(peek().getTokenType() != RIGHT_PAREN){
                expect(COMMA);
                EXPRESSION(parameters);
            }
        }

        expect(RIGHT_PAREN);

        container.emplace_back(new SelfMethodAccess(method, parameters));
    }

    void Parser::LET(std::vector<Expression*> &container) {
        expect(TokenType::LET);

        Token objId = expect(OBJ_ID);

        expect(COLON);

        Token typeId = expect(TYPE_ID);

        std::vector<InnerLet*> innerLets;
        if(peek().getTokenType() == ASSIGN){
            expect(ASSIGN);

            std::vector<Expression*> assignExpressions;
            EXPRESSION(assignExpressions);

            if(assignExpressions.empty()){
                innerLets.emplace_back(
                        new InnerLet(objId.getLexeme(),
                                 typeId.getLexeme()));
            }
            else{
                innerLets.emplace_back(
                        new InnerLet(objId.getLexeme(),
                                 typeId.getLexeme(),
                                 assignExpressions.front()));
            }
        }

        if(peek().getTokenType() == COMMA){
            while(peek().getTokenType() != IN){
                expect(COMMA);

                Token objId1 = expect(OBJ_ID);

                expect(COLON);

                Token typeId1 = expect(TYPE_ID);

                if(peek().getTokenType() == ASSIGN){
                    expect(ASSIGN);

                    std::vector<Expression*> assignExpressions1;
                    EXPRESSION(assignExpressions1);

                    innerLets.emplace_back(
                            new InnerLet(objId1.getLexeme(),
                                     typeId1.getLexeme(),
                                     assignExpressions1.front()));
                }
            }
        }

        expect(IN);

        std::vector<Expression*> inExpressions;
        EXPRESSION(inExpressions);

        container.emplace_back(new Let(innerLets, inExpressions.front()));
    }

    void Parser::CASE(std::vector<Expression*> &container) {
        expect(TokenType::CASE);

        std::vector<Expression*> caseExpressions;
        EXPRESSION(caseExpressions);

        expect(OF);

        Token objId1 = expect(TokenType::OBJ_ID);

        expect(COLON);

        Token typeId1 = expect(TokenType::TYPE_ID);

        expect(TokenType::EQ);
        expect(TokenType::GT);

        std::vector<Expression*> caseAction1;
        EXPRESSION(caseAction1);

        expect(SEMICOLON);

        std::vector<CaseAction*> actions;
        actions.emplace_back(new CaseAction(objId1.getLexeme(), typeId1.getLexeme(), caseAction1.front()));

        if(peek().getTokenType() == OBJ_ID){
            while (peek().getTokenType() != ESAC){
                Token objIdN = expect(TokenType::OBJ_ID);

                expect(COLON);

                Token typeIdN = expect(TokenType::TYPE_ID);

                expect(TokenType::EQ);
                expect(TokenType::GT);

                std::vector<Expression*> caseActionN;
                EXPRESSION(caseActionN);

                expect(SEMICOLON);

                actions.emplace_back(new CaseAction(objIdN.getLexeme(), objIdN.getLexeme(), caseActionN.front()));
            }
        }

        expect(ESAC);

        container.emplace_back(new Case(caseExpressions.front(), actions));
    }

    void Parser::NEW(std::vector<Expression*> &container) {
        expect(TokenType::NEW);

        Token typeId = expect(TYPE_ID);

        container.emplace_back(new New(typeId.getLexeme()));
    }

    void Parser::IS_VOID(std::vector<Expression*> &container) {
        expect(TokenType::ISVOID);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        container.emplace_back(new IsVoid(expressions.front()));
    }

    void Parser::PLUS(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::PLUS, left, container);
    }

    void Parser::MINUS(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::MINUS, left, container);
    }

    void Parser::STAR(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::STAR, left, container);
    }

    void Parser::SLASH(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::SLASH, left, container);
    }

    void Parser::TILDE(std::vector<Expression*> &container) {
        expect(TokenType::TILDE);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        container.emplace_back(new Tilde(expressions.front()));
    }

    void Parser::LESS_THAN(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::LT, left, container);
    }

    void Parser::LESS_THAN_EQ(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::LTOE, left, container);
    }

    void Parser::EQ(Expression* left, std::vector<Expression*> &container) {
        MATH_BINOP(TokenType::EQ, left, container);
    }

    void Parser::MATH_BINOP(TokenType tokenType, Expression* left, std::vector<Expression*> &container) {
        Token token = expect(tokenType);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        Expression* right = expressions.front();

        container.emplace_back(new MathBinop(token.getLexeme(), tokenType, left, right));
    }

    void Parser::NOT(std::vector<Expression*> &container) {
        expect(TokenType::NOT);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        container.emplace_back(new Not(expressions.front()));
    }

    void Parser::PAREN(std::vector<Expression*> &container) {
        expect(LEFT_PAREN);

        std::vector<Expression*> expressions;
        EXPRESSION(expressions);

        expect(RIGHT_PAREN);

        container.emplace_back(expressions.front());
    }

    void Parser::ID(std::vector<Expression*> &container) {
        Token idToken = expect(TokenType::OBJ_ID);

        container.emplace_back(new Id(idToken.getLexeme()));
    }

    void Parser::INTEGER(std::vector<Expression*> &container) {
        Token numToken = expect(TokenType::NUMBER);
        int number = std::stoi(numToken.getLexeme());

        container.emplace_back(new Integer(number));
    }

    void Parser::STRING(std::vector<Expression*> &container) {
        Token strToken = expect(TokenType::STRING);

        container.emplace_back(new String(strToken.getLexeme()));
    }

    void Parser::TRUE(std::vector<Expression*> &container) {
        expect(TokenType::TRUE);
        container.emplace_back(new Boolean(true));
    }

    void Parser::FALSE(std::vector<Expression*> &container) {
        expect(TokenType::FALSE);
        container.emplace_back(new Boolean(false));
    }

    void Parser::EXPRESSION(std::vector<Expression*> &container) {
        Expression* left = nullptr;
        std::vector<Expression*> expressions;
        bool isMethodDispatch = false;

        switch (peek().getTokenType()) {
            case TokenType::SELF:
                expect(TokenType::SELF);

                if(peek().getTokenType() != DOT){
                    expressions.emplace_back(new Self());
                    break;
                }

                next();

                if(peek().getTokenType() == TokenType::OBJ_ID){
                    Token methodToken = expect(TokenType::OBJ_ID);

                    SELF_METHOD_ACCESS(methodToken.getLexeme(), expressions);

                    break;
                }

                break;
            case TokenType::OBJ_ID:
                if(peek(2).getTokenType() == ASSIGN)
                    ASSIGNMENT(expressions);
                else if(peek(2).getTokenType() == LEFT_PAREN){
                    Token methodToken = expect(TokenType::OBJ_ID);

                    SELF_METHOD_ACCESS(methodToken.getLexeme(), expressions);
                } else{
                    std::vector<Expression*> ids;
                    ID(ids);
                    left = ids.back();
                }
                break;
            case TokenType::IF:
                IF(expressions);
                break;
            case TokenType::WHILE:
                WHILE(expressions);
                break;
            case TokenType::LEFT_BRACE:
                BLOCK(expressions);
                break;
            case TokenType::LET:
                LET(expressions);
                break;
            case TokenType::CASE:
                CASE(expressions);
                break;
            case TokenType::NEW:
                NEW(expressions);
                break;
            case TokenType::ISVOID:
                IS_VOID(expressions);
                break;
            case TokenType::TILDE:
                TILDE(expressions);
                break;
            case TokenType::NOT:
                NOT(expressions);
                break;
            case TokenType::LEFT_PAREN:
                PAREN(expressions);
                break;
            case TokenType::NUMBER:
                INTEGER(expressions);
                break;
            case TokenType::STRING:
                STRING(expressions);
                break;
            case TokenType::TRUE:
                TRUE(expressions);
                break;
            case TokenType::FALSE:
                FALSE(expressions);
                break;
        }

        if(left == nullptr && expressions.empty()) return;
        if(left == nullptr && !expressions.empty())
            left = expressions.back();

        if(peek().getTokenType() == DOT){
            next();
            METHOD_ACCESS(left, expressions);
            isMethodDispatch = true;
        }
        else if(peek().getTokenType() == AT){
            next();
            AT_METHOD_ACCESS(left, expressions);
            isMethodDispatch = true;
        }

        Expression* expr = isMethodDispatch ? expressions.back() : left;

        switch (peek().getTokenType()) {
            case TokenType::PLUS:
                PLUS(expr, container);
                break;
            case TokenType::MINUS:
                MINUS(expr, container);
                break;
            case TokenType::STAR:
                STAR(expr, container);
                break;
            case TokenType::SLASH:
                SLASH(expr, container);
                break;
            case TokenType::LT:
                LESS_THAN(expr, container);
                break;
            case TokenType::LTOE:
                LESS_THAN_EQ(expr, container);
                break;
            case TokenType::EQ:
                EQ(expr, container);
                break;
            default:
                container.emplace_back(expr);
                break;
        }
    }

    // End of Expressions
    // End of AST Resolver
} // CoolCompiler