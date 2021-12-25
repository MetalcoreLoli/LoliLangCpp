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

TEST_F (BodyTests, BodyExpression_WithoutEndWord_ThrowsRuntimeError) {
    std::string code = "with";
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act und assert 
    ASSERT_THROW(d.growTree(), std::runtime_error);
}

TEST_F (BodyTests, BodyExpression_WithTwoLambdasInside_ReturnsValidTree) {
    std::string code = "with n => 1 + 5; a => \"hello, world\" end";
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act 
    std::string result = loli::unwrap<void, std::string>(d.growTree()->visit(&_ast));

    //assert 
    ASSERT_STREQ(
            result.c_str(), 
            "(with (define (n) (+ 5.000000 1.000000)) (define (a) \"hello, world\"))"
    );
}
#endif // __LOLI_BODY_TESTS__
