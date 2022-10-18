//
// Created by luoli on 2022/10/17.
//

#include "gtest/gtest.h"
#include "../Src/Lexer/Lexer.h"

namespace {

    void run(const std::string &name){
        std::ostringstream stream;
        stream << "../Examples/" << name << ".cl";

        CoolCompiler::Lexer lexer = CoolCompiler::Lexer(stream.str());
        lexer.doScan();

        EXPECT_EQ(lexer.getErrors().size(), 0);
    }

    TEST(LexerTest, arith) {
        run("arith");
    }

    TEST(LexerTest, atoi) {
        run("atoi");
    }

    TEST(LexerTest, atoi_test) {
        run("atoi_test");
    }

    TEST(LexerTest, book_list) {
        run("book_list");
    }

    TEST(LexerTest, cells) {
        run("cells");
    }

    TEST(LexerTest, classes) {
        run("classes");
    }

    TEST(LexerTest, complex) {
        run("complex");
    }

    TEST(LexerTest, cool) {
        run("cool");
    }

    TEST(LexerTest, factorial) {
        run("factorial");
    }

    TEST(LexerTest, graph) {
        run("graph");
    }

    TEST(LexerTest, hairyscary) {
        run("hairyscary");
    }

    TEST(LexerTest, hello_world) {
        run("hello_world");
    }

    TEST(LexerTest, helloWorld) {
        run("helloWorld");
    }

    TEST(LexerTest, io) {
        run("io");
    }

    TEST(LexerTest, lam) {
        run("lam");
    }

    TEST(LexerTest, life) {
        run("life");
    }

    TEST(LexerTest, list) {
        run("list");
    }

    TEST(LexerTest, new_complex) {
        run("new_complex");
    }

    TEST(LexerTest, palindrome) {
        run("palindrome");
    }

    TEST(LexerTest, primes) {
        run("primes");
    }

    TEST(LexerTest, sort_list) {
        run("sort_list");
    }

}