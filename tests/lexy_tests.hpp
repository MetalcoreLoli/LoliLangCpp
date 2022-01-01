#ifndef __LEXY_TESTS__
#define __LEXY_TESTS__

#include "loliLang/daphnie.h"
#include "loliLang/lexy.h"
#include <cmath>
#include <exception>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <loliLang/common.h>

class LexyTests : public ::testing::Test {
    protected:
        loli::Lexy _lex{};
};

TEST_F (LexyTests, visitBinaryExpression_WithOneAddOne_ReturnsTwo) {
    loli::Daphnie d{"1 + 1"};

    //act 
    auto result = loli::unwrap <void, float> (d.growTree()->visit(&_lex));


    //assert 
    ASSERT_GT (0.05f, std::abs(2.0f - result));
}

TEST_F (LexyTests, visitBinaryExpression_WithOneEqOne_ThrowsRuntimeError) {
    std::string code = "1==1";

    //act
    auto act = [](loli::Daphnie d, loli::IVisitor *visitor) {
        d.growTree()->visit(visitor);
    };

    //assert
    ASSERT_THROW(act(loli::Daphnie{code}, &_lex), std::runtime_error);
    try {
        act(loli::Daphnie{code}, &_lex);
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "operator `==` is not implemented for type `Number`");
    }
}

#endif //__LEXY_TESTS__
