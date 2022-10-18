//
// Created by luoli on 2022/10/17.
//

#include "gtest/gtest.h"
#include "../Src/Utils/StringUtils.h"
#include "../Src/Lexer/Lexer.h"

namespace {
    TEST(StringUtilsTest, Replace){
        //auto lexer = CoolCompiler::Lexer("Examples/hello_world.cl");
        //lexer.doScan();

        std::string testStr = "This is a test str test!";

        StringUtils::replace(testStr, "test", "TEST");
        EXPECT_EQ(testStr, "This is a TEST str test!");

        StringUtils::replace(testStr, "TEST", "XXXX");
        EXPECT_EQ(testStr, "This is a XXXX str test!");
    }

    TEST(StringUtilsTest, ReplaceAll){
        std::string testStr = "This is a test str test!";

        StringUtils::replaceAll(testStr, "test", "TEST");
        EXPECT_EQ(testStr, "This is a TEST str TEST!");

        StringUtils::replaceAll(testStr, "TEST", "XXXX");
        EXPECT_EQ(testStr, "This is a XXXX str XXXX!");
    }
}