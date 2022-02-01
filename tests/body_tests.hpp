#ifndef __LOLI_BODY_TESTS__
#define __LOLI_BODY_TESTS__

#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include <loliLang/common.h>
#include <string>

class BodyTests : public ::testing::Test {
    protected:
        loli::Lexer _lex{};
        loli::ASTAsString _ast{};
};

TEST_F (BodyTests, BodyExpression_WithoutEndWord_ThrowsSyntaxErrorExceptionWithCustomMessage) {
    std::string code = "with";
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act und assert 
    ASSERT_THROW(d.growTree(), loli::SyntaxErrorException);

    try {
        loli::Lexer l{};
        loli::Daphnie dd{l.lineToTokens("with")};
        dd.growTree();
    } catch (const loli::SyntaxErrorException& ex) {
        ASSERT_STREQ(ex.what(), "There is no `end` keyword at the end of `with` block");
    }
}

TEST_F (BodyTests, BodyExpression_WithTwoLambdasInside_ReturnsValidTree) {
    std::string code = "with n => 1 + 5; a => \"hello, world\" end";
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert 
    ASSERT_STREQ(
            result.c_str(), 
            "(with (define (n) (+ 5.000000 1.000000)) (define (a) \"hello, world\"))"
    );
}

TEST_F(BodyTests, BodyExpr_AsLambdaExpresionBody_ReturnsValidTree) {
    std::string code = "a => with b = 1+1; c = 1-1; end";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act 
    std::string result = (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ (
            result.c_str(), 
            "(define (a) (with (define (b) (+ 1.000000 1.000000)) (define (c) (- 1.000000 1.000000))))");
}
#endif // __LOLI_BODY_TESTS__
