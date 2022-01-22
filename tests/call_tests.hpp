#ifndef __LOLI_CALL_TESTS__
#define __LOLI_CALL_TESTS__

#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <loliLang/common.h>

class CallTests : public ::testing::Test {
    protected:
        loli::Lexy _lexy{};
};

TEST_F (CallTests, Call_FuncWhichContainsOnePlusOneExpression_ReturnsTwoAsResult) {
    auto codeOfFunc = "a = 1 + 1";
    auto call = "a";
    loli::Daphnie d{call};
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&_lexy);
    auto result = loli::unwrap <void, float> (d.growTree()->visit(&_lexy));

    //assert 
    ASSERT_EQ (2.0f, result);

}

TEST_F (CallTests, Call_FuncWithOneArgInWhichPassedOneUndUsedInsideFuncsBody_ReturnsTen) {
    auto codeOfFunc = "id a => a + 9";
    auto call = "id 1";
    loli::Daphnie d{call};
    loli::Daphnie dd{codeOfFunc};
    
    //act 
    dd.growTree() -> visit(&_lexy);
    auto result = loli::unwrap <void, float> (d.growTree() -> visit (&_lexy));

    //assert 
    ASSERT_EQ (10.0f, result);
}
#endif // __LOLI_CALL_TESTS__
