#ifndef __LOLI_AST_TESTS__
#define __LOLI_AST_TESTS__

#include "loliLang/daphnie.h"
#include "loliLang/expression.h"
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
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(if 1 0 1)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidBinaryExpression_ReturnsValidASTAsStringValue) {
    std::string code = "1+2";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(+ 2.000000 1.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidUnaryExpression_ReturnsValidASTAsStringValue) {
    std::string code = "-12";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(- 12.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidIdentifierExpression_ReturnsValidASTAsStringValue) {
    std::string code = "add a b => a + b";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(define (add a b) (+ b a))";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidLambdaExpression_ReturnsValidASTAsStringValue) {
    std::string code = "add => if (true) false else false";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(define (add) (if 1 0 0))";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}


TEST_F (ASTAsStringTests, Visit_WithValidGroupingExpression_ReturnsValidASTAsStringValue) {
    std::string code = "(7+8)";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(+ 8.000000 7.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}

TEST_F (ASTAsStringTests, Visit_WithValidLCurlGroupingExpression_ReturnsValidASTAsStringValue) {
    std::string code = "{7+8}";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();
    std::string expect = "(+ 8.000000 7.000000)";
    
    //assert
    ASSERT_STREQ(result.c_str(), expect.c_str());
}


TEST_F (ASTAsStringTests, Visit_WithValidAddExpresionInsideBodyExpression_ReturnVailASTAsStringValue) {
    std::string code = "with a = 1 + 1; end";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ(result.c_str(), "(with (define (a) (+ 1.000000 1.000000)))");

}

TEST_F (ASTAsStringTests, Visit_WithEmptyClassBody_ReturnsValidAST) {
    std::string code = "class name;";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ (result.c_str(), "(class (name))");
}

TEST_F (ASTAsStringTests, Visit_WithTwoLambdaInsideClassBody_ReturnsValidAST) {
    std::string code = "class Loli with age = 16; Age => age; end";
    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ (result.c_str(), "(class (Loli) (define (age) 16.000000) (define (Age) age))");
}

TEST_F (ASTAsStringTests, Visit_WithForValidForExpression_ReturnsValidAST) {
    std::string code = "for i = 0; i < 10; i = i + 1; with 1 end";

    loli::Daphnie d {_lexy.lineToTokens(code)};

    //act 
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ(
            result.c_str(), 
            "(for (define (i) 0.000000) (< 10.000000 i) (define (i) (+ 1.000000 i)) (with 1.000000))");
}


TEST_F (ASTAsStringTests, Visit_CallExpressionWithoutArgs_ReturbsValidASTAsString) {
    auto call = loli::ExpressionFactory::CallWithoutArgs("d");

    //act 
    auto result = (call->visit(&_ast)).Unwrap<std::string>();

    //assert 
    ASSERT_STREQ (result.c_str(), "(call d)");
}

TEST_F (ASTAsStringTests, Visit_CallExpressionWithNumberLiteralUndFunc_ReturbsValidASTAsString) {
    auto call = loli::ExpressionFactory::Call("add", {
            loli::ExpressionFactory::Lambda("id", loli::ExpressionFactory::Number(9).get()).get(),
            loli::ExpressionFactory::Number (1).get()
            });

    //act 
    auto result = (call->visit(&_ast)).Unwrap<std::string>();

    //assert 
    ASSERT_STREQ (result.c_str(), "(call add (define (id) 9.000000) 1.000000)");
}
#endif
