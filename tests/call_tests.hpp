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

TEST_F (CallTests, Call_WithWrongAmountOfArgsPasssedThroughtIt_ThrowRutimeError) {
    auto codeOfFunc = "add a b => a + b";
    auto call = "add 1 2 3";
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&_lexy);
    auto act = [this](loli::Daphnie d)  {
        d.growTree()->visit(&_lexy);
    };

    //assert 
    ASSERT_THROW (act(loli::Daphnie{call}), std::runtime_error);
    try {
        act (loli::Daphnie{call});
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "There is a extra arg in call of `add` function");
    }
    try {
        act (loli::Daphnie{"add 1"});
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "There is a missing arg in call of `add` function");
    }

}

TEST_F (CallTests, Call_WithRecurtionCall_ReturnsFive) {
    auto codeOfFunc = "add a b => if (a < b) add b a else 5";
    auto call = "add 1 2";
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&_lexy);
    auto result = loli::unwrap<void, float> (loli::Daphnie{call}.growTree()->visit(&_lexy));

    //assert 
    ASSERT_EQ(5.0f,result);

}
#endif // __LOLI_CALL_TESTS__
