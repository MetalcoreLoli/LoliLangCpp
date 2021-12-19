#include <gtest/gtest.h>
#include <loliLang/common.h>

TEST(DaphnieTests, test_name) {
    loli::Lexer lex;
    std::string code = "(1+1*7)";
    auto tokens = lex.lineToTokens (code);
    loli::Daphnie d{tokens};
    EXPECT_EQ(1,1);
}

int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
