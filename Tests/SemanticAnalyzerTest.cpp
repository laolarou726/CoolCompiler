//
// Created by luoli on 2022/10/30.
//

#include "gtest/gtest.h"
#include "../Src/Lexer/Lexer.h"
#include "../Src/Parser/Parser.h"
#include "../Src/Semantic/SemanticAnalyzer.h"

namespace {

    void run(const std::string &name){
        std::ostringstream stream;
        stream << "../Examples/" << name << ".cl";

        CoolCompiler::Lexer lexer = CoolCompiler::Lexer(stream.str());
        lexer.doScan();

        EXPECT_EQ(lexer.getErrors().size(), 0);

        CoolCompiler::Parser parser = CoolCompiler::Parser(lexer.getTokens());
        parser.parse();

        EXPECT_EQ(parser.getErrors().size(), 0);

        CoolCompiler::SemanticAnalyzer analyzer = CoolCompiler::SemanticAnalyzer(parser.getParseTree());
        analyzer.doCheck();
    }

    /*
    TEST(SemanticAnalyzerTest, arith) {
        run("arith");
    }

    TEST(SemanticAnalyzerTest, atoi) {
        run("atoi");
    }

    TEST(SemanticAnalyzerTest, atoi_test) {
        run("atoi_test");
    }

    TEST(SemanticAnalyzerTest, book_list) {
        run("book_list");
    }

    TEST(SemanticAnalyzerTest, cells) {
        run("cells");
    }

    TEST(SemanticAnalyzerTest, classes) {
        run("classes");
    }

    TEST(SemanticAnalyzerTest, complex) {
        run("complex");
    }

    TEST(SemanticAnalyzerTest, cool) {
        run("cool");
    }

    TEST(SemanticAnalyzerTest, factorial) {
        run("factorial");
    }

    TEST(SemanticAnalyzerTest, graph) {
        run("graph");
    }

    TEST(SemanticAnalyzerTest, hairyscary) {
        run("hairyscary");
    }

    TEST(SemanticAnalyzerTest, hello_world) {
        run("hello_world");
    }

    TEST(SemanticAnalyzerTest, helloWorld) {
        run("helloWorld");
    }

    TEST(SemanticAnalyzerTest, io) {
        run("io");
    }

    TEST(SemanticAnalyzerTest, lam) {
        run("lam");
    }

    TEST(SemanticAnalyzerTest, life) {
        run("life");
    }
     */



    /*
    TEST(SemanticAnalyzerTest, list) {
        run("list");
    }

    TEST(SemanticAnalyzerTest, new_complex) {
        run("new_complex");
    }

    TEST(SemanticAnalyzerTest, palindrome) {
        run("palindrome");
    }
*/
    TEST(SemanticAnalyzerTest, primes) {
        run("primes");
    }
/*
    TEST(SemanticAnalyzerTest, sort_list) {
        run("sort_list");
    }
*/
}
