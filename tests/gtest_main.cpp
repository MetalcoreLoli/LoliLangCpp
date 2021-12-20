#include <gtest/gtest.h>
#include <loliLang/common.h>

class DaphnieTests : public ::testing::Test {
    protected:
        loli::Lexer _lex{};
        loli::ASTAsString _ast{};

        void SetUp () override {
        }

};

TEST_F(DaphnieTests, BinaryExpression_OnePlusSevenMulNine_ReturnsValidSting) {
    std::string code = "1+7*9";
    loli::Daphnie d {_lex.lineToTokens(code)};
    
    // act
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));

    // assert
    ASSERT_STREQ(result.c_str(), "(+ (* 9.000000 7.000000) 1.000000)");
}

TEST_F(DaphnieTests, UnaryExpression_MinusOne_ReturnsValidBinaryTree) {
    std::string code = "-1";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));

    //assert
    ASSERT_STREQ(result.c_str(), "(- 1.000000)");
}

TEST_F (DaphnieTests, GroupingExpression_WithTwoBinaryExpressionInside_ReturnsValidString) {
    std::string code = "(1+1*7)";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect ="(+ (* 7.000000 1.000000) 1.000000)"; 

    EXPECT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (DaphnieTests, IfExpression_WithMissingRparen_ThrowsRuntimeError) {
    std::string code = "if(";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    EXPECT_ANY_THROW(d.growTree()->visit(&_ast));
}

TEST_F (DaphnieTests, GroupingExpression_WithIfStatment_ReturnsValidString) {
    std::string code = "if (true) (true) else (false)";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(if (1) 1 0)";

    EXPECT_STREQ(result.c_str(), expect.c_str());
}

int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
