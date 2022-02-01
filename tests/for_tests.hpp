#ifndef __LOLI_FOR_TESTS__
#define __LOLI_FOR_TESTS__

#include <exception>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <loliLang/common.h>


class ForExpressionTests : public ::testing::Test {
    protected:
        loli::Lexer _lex{};
        loli::ASTAsString _ast{};
};

TEST_F(ForExpressionTests, ForExpression_WithMissingAllParts_ReturnValidTree) {
    loli::Daphnie d {"for"};

    //act/assert 
    ASSERT_THROW(d.growTree(), loli::SyntaxErrorException);
}

TEST_F(ForExpressionTests, ForExpression_WithMissingBody_ThrowsSyntaxErrorWithCustomMessage) {
    std::string code = "for i = 0; i != 4; i = i + 1";
    loli::Daphnie d {code};

    //act/assert 
    ASSERT_THROW (d.growTree(), loli::SyntaxErrorException);
    try {
        loli::Daphnie dd{code};
        dd.growTree()->visit(&_ast);
    } catch (const std::exception& ex) {
        ASSERT_STREQ (ex.what(), "missing body after `for`");
    }
}


TEST_F(ForExpressionTests, ForExpression_WithValidParts_ReturnValidTree) {
    std::string code = "for i = 0; i < 10; i = i + 1; with 1 end";
    loli::Daphnie d {code};

    //act 
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ(result.c_str(), "(for (define (i) 0.000000) (< 10.000000 i) (define (i) (+ 1.000000 i)) (with 1.000000))");
}

#endif // __LOLI_FOR_TESTS__
