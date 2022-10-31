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

    TEST(SemanticAnalyzerTest, arith) {
        run("arith");
    }

}
