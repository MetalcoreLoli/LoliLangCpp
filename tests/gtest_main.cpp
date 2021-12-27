#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include <loliLang/common.h>


#include "ast_tests.hpp"
#include "class_tests.hpp"
#include "body_tests.hpp"
#include "exception_tests.hpp"

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

TEST_F(DaphnieTests, UnaryExpression_NegativeOneMinusFourInsideGrouping_ReturnsValidBinaryTree) {
    std::string code = "(-1-4)";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));

    //assert
    ASSERT_STREQ(result.c_str(), "(- 4.000000 (- 1.000000))");
}

TEST_F (DaphnieTests, GroupingExpression_WithTwoBinaryExpressionInside_ReturnsValidString) {
    std::string code = "(1+7*1)";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect ="(+ (* 1.000000 7.000000) 1.000000)"; 

    EXPECT_STREQ(result.c_str(), expect.c_str());
}

TEST_F(DaphnieTests, GroupingExpression_Empty_ThrowsRuntimeError) {
    std::string code = "()";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d {tokens};

    //act
    ASSERT_THROW(d.growTree(), std::runtime_error);
}

TEST_F(DaphnieTests, GroupingExpression_WithoutRparen_ThrowsRuntimeError) {
    std::string code = "(";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d {tokens};

    //act
    ASSERT_THROW(d.growTree(), std::runtime_error);
}


TEST_F(DaphnieTests, GroupingExpression_EmptyLCURL_ThrowsRuntimeError) {
    std::string code = "{}";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d {tokens};

    //act
    ASSERT_THROW(d.growTree(), std::runtime_error);
}

TEST_F(DaphnieTests, GroupingExpression_WithoutLCURL_ThrowsRuntimeError) {
    std::string code = "{";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d {tokens};

    ASSERT_THROW(d.growTree(),std::runtime_error);
}

TEST_F(DaphnieTests, UnaryExpression_NegativeOneMinusFourInsideLCurlGrouping_ReturnsValidBinaryTree) {
    std::string code = "{-1-4}";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));

    //assert
    ASSERT_STREQ(result.c_str(), "(- 4.000000 (- 1.000000))");
}

TEST_F (DaphnieTests, IfExpression_WithMissingRparen_ThrowsRuntimeError) {
    std::string code = "if(";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    EXPECT_ANY_THROW(d.growTree()->visit(&_ast));
}

TEST_F (DaphnieTests, IfExpression_WithoutCondition_ThrowsLogicError) {
    loli::Daphnie d {_lex.lineToTokens("if ()")};

    ASSERT_THROW(d.growTree(), std::logic_error);
}


TEST_F (DaphnieTests, IfExpression_WithMissingThenBranch_ThrowsInvalidArgument) {
    std::string code = "if (true) ";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d{tokens};

    ASSERT_THROW(d.growTree(), std::invalid_argument);
}

TEST_F (DaphnieTests, IfExpression_WithMissingElseBranch_ThrowsElseBranchException) {
    std::string code = "if (true) (1+7)";
    auto tokens = _lex.lineToTokens(code);
    loli::Daphnie d{tokens};

    ASSERT_THROW(
            d.growTree(),
            loli::ElseBranchException);
}

TEST_F (DaphnieTests, GroupingExpression_WithIfStatment_ReturnsValidString) {
    std::string code = "if (true) (true) else (4-(-1))";
    auto tokens = _lex.lineToTokens (code);
    loli::Daphnie d{tokens};

    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(if 1 1 (- (- 1.000000) 4.000000))";

    EXPECT_STREQ(result.c_str(), expect.c_str());
}


TEST_F (DaphnieTests, Daphnie_WithValidCodeStringPassedThroughtTheConstructor_TranslatesItIntoValidTree) {
    loli::Daphnie d{"class name;"};

    //act 
    std::string result = loli::unwrap <void, std::string> (d.growTree()->visit(&_ast));
    
    //assert 
    ASSERT_STREQ (result.c_str(), "(class (name))");
}

int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
