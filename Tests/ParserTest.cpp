//
// Created by luoli on 2022/10/17.
//

#include "gtest/gtest.h"
#include "../Src/Lexer/Lexer.h"
#include "../Src/Parser/Parser.h"

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
    }
/*
    TEST(ParserTest, arith) {
        run("arith");
    }

    TEST(ParserTest, atoi) {
        run("atoi");
    }

    TEST(ParserTest, atoi_test) {
        run("atoi_test");
    }

    TEST(ParserTest, book_list) {
        run("book_list");
    }

    TEST(ParserTest, cells) {
        run("cells");
    }

    TEST(ParserTest, classes) {
        run("classes");
    }

    TEST(ParserTest, complex) {
        run("complex");
    }

    TEST(ParserTest, cool) {
        run("cool");
    }

    TEST(ParserTest, factorial) {
        run("factorial");
    }

    TEST(ParserTest, graph) {
        run("graph");
    }

    TEST(ParserTest, hairyscary) {
        run("hairyscary");
    }

    TEST(ParserTest, hello_world) {
        run("hello_world");
    }

    TEST(ParserTest, helloWorld) {
        run("helloWorld");
    }

    TEST(ParserTest, io) {
        run("io");
    }

    TEST(ParserTest, lam) {
        run("lam");
    }

    TEST(ParserTest, life) {
        run("life");
    }

    TEST(ParserTest, list) {
        run("list");
    }

    TEST(ParserTest, new_complex) {
        run("new_complex");
    }

    TEST(ParserTest, palindrome) {
        run("palindrome");
    }
    */
    TEST(ParserTest, primes) {
        run("primes");
    }
/*
    TEST(ParserTest, sort_list) {
        run("sort_list");
    }
*/
}
