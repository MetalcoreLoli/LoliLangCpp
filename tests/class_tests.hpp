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

TEST_F (ClassExpressionTests, WithoutName_ThrowsSyntaxErrorWithCustomMessage) {
    std::string code = "class";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    ASSERT_THROW(d.growTree(), loli::SyntaxErrorException);
    try {
        loli::Daphnie dd {_lex.lineToTokens(code)};
        d.growTree();
    }
    catch (const loli::SyntaxErrorException& ex) {
        ASSERT_STREQ (ex.what(), "there is no name of class");
    }
}

TEST_F (ClassExpressionTests, WithoutSemiOrWith_ThrowsSyntaxErrorWithCustomErrorMessage) {
    std::string code = "class Loli";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    ASSERT_THROW(d.growTree(), loli::SyntaxErrorException);

    try {
        loli::Daphnie dd {_lex.lineToTokens(code)};
        d.growTree();
    } catch (const std::exception& ex) {
        ASSERT_STREQ (ex.what(), "exceped `with` or ';' after class-name");
    }
}

TEST_F(ClassExpressionTests, ClassExpression_EmptyClass_ReturnsValidExpressionTree) {
    std::string code = "class Loli;"; 
    loli::Daphnie d{_lex.lineToTokens(code)};

    //act
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ(result.c_str(), "(class (Loli))");
}

TEST_F (ClassExpressionTests, ClassExpression_ClassWithProperties_ReturnsValidTree) {
    std::string code = "class Loli with Name => \"Haku\"; Age => 16; end";
    loli::Daphnie d {_lex.lineToTokens(code)};

    //act
    std::string result =  (d.growTree()->visit(&_ast)).Unwrap<std::string>();

    //assert
    ASSERT_STREQ(result.c_str(), "(class (Loli) (define (Name) \"Haku\") (define (Age) 16.000000))");
}

#endif
