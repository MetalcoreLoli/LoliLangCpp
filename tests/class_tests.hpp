#ifndef __LOLI_CLASS_TESTS__
#define __LOLI_CLASS_TESTS__

#include <exception>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <loliLang/common.h>

class ClassExpressionTests: public ::testing::Test {
    protected:
        loli::Lexer _lex{};
        loli::ASTAsString _ast{};
};

TEST_F (ClassExpressionTests, WithoutName_ThrowsLogicalError) {
    std::string code = "class";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    ASSERT_THROW(d.growTree(), std::logic_error);
}

TEST_F (ClassExpressionTests, WithoutSemiOrLCurl_ThrowsInvalidArgument) {
    std::string code = "class Loli";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    ASSERT_THROW(d.growTree(), std::invalid_argument);
}

TEST_F(ClassExpressionTests, ClassExpression_EmptyClass_ReturnsValidExpressionTree) {
    std::string code = "class Loli;"; 
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act
    std::string result = loli::unwrap<void, std::string> (d.growTree()->visit(&_ast));

    //assert
    ASSERT_STREQ(result.c_str(), "(class (Loli))");
}

TEST_F (ClassExpressionTests, ClassExpression_ClassWithProperties_ReturnsValidTree) {
    std::string code = "class Loli { Name => \"Haku\"; Age => 16; }";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    std::string result = loli::unwrap <void, std::string> (d.growTree()->visit(&_ast));

    //assert
    ASSERT_STREQ(result.c_str(), "(class (Loli) (define (Name) \"Haku\") (define (Age) 16.000000))");
}

#endif
