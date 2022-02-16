#ifndef __LEXER_TESTS__
#define __LEXER_TESTS__

#include "loliLang/lexer.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <loliLang/common.h>

class LexerTests : public ::testing::Test {

};

TEST_F(LexerTests, LineToTokens_WithWhereKeyWord_ReturnsTokenWithWhereLexem) {
    auto code = "where";

    //act 
    auto result = loli::Lexer().lineToTokens(code).at(0);

    //assert 
    ASSERT_EQ(result.forma(),loli::Forma::WHERE);
}
#endif //__LEXER_TESTS__
