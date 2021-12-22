#ifndef __LOLI_AST_TESTS__
#define __LOLI_AST_TESTS__

#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include <loliLang/common.h>
#include <string>


class ASTAsStringTests : public ::testing::Test {
    protected:
        loli::ASTAsString _ast{};
        loli::Lexer _lexy{};
};


TEST_F (ASTAsStringTests, Visit_WithValidIfExpression_ReturnsValidASTAsStringValue) {
    std::string code = "if (true) false else true";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(if 1 0 1)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidBinaryExpression_ReturnsValidASTAsStringValue) {
    std::string code = "1+2";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(+ 2.000000 1.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidUnaryExpression_ReturnsValidASTAsStringValue) {
    std::string code = "-12";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(- 12.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidIdentifierExpression_ReturnsValidASTAsStringValue) {
    std::string code = "add a b => a + b";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(define (add a b) (+ b a))";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidLambdaExpression_ReturnsValidASTAsStringValue) {
    std::string code = "add => if (true) false else false";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(define (add) (if 1 0 0))";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}


TEST_F (ASTAsStringTests, Visit_WithValidGroupingExpression_ReturnsValidASTAsStringValue) {
    std::string code = "(7+8)";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));
    std::string expect = "(+ 8.000000 7.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

#endif
