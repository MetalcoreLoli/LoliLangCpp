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

TEST_F (CallTests, Call_FibNumberTenWithRecursiveHelperFunctionCall_ReturnsFityFive) {
    auto fibHelper  = "fibHelper a b n => if (0 < n) fibHelper (a + b) a (1 - n) else a";
    auto fib        = "fib n => fibHelper 0 1 n";

    //act 
    loli::Daphnie {fibHelper}.growTree()->visit(&_lexy);
    loli::Daphnie {fib}.growTree()->visit(&_lexy);

    auto result = loli::Daphnie {"fib 10"}.growTree()->visit(&_lexy).Unwrap<float>();

    //assert 
    ASSERT_EQ (55.0f, result);
    loli::mem::Environment::Instance().Clear();
}

TEST_F (CallTests, Call_With3Args_ReturnsRigthValue) {
    auto codeOfFunc    = "id a b c => if (a > 0) a else if (b > 0) b else c";

    //act
    loli::Daphnie{codeOfFunc}.growTree()->visit(&_lexy);
    auto resultA = 
         (loli::Daphnie{"id 1 2 3"}.growTree()->visit(&_lexy)).Unwrap<float>();
    auto resultB = 
         (loli::Daphnie{"id 0 2 3"}.growTree()->visit(&_lexy)).Unwrap<float>();
    auto resultC = 
         (loli::Daphnie{"id 0 0 3"}.growTree()->visit(&_lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ(1.0f,resultA);
    ASSERT_EQ(2.0f,resultB);
    ASSERT_EQ(3.0f,resultC);
    
    loli::mem::Environment::Instance().Clear();
}


TEST_F (CallTests, Call_FuncWithOneArgInWhichPassedOneUndUsedInsideFuncsBody_ReturnsTen) {
    auto codeOfFunc = "id a => a + 9";
    auto call = "id 1";
    loli::Daphnie d{call};
    loli::Daphnie dd{codeOfFunc};
    
    //act 
    dd.growTree() -> visit(&_lexy);
    auto result =  (d.growTree() -> visit (&_lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ (10.0f, result);
    loli::mem::Environment::Instance().Clear();
}
TEST_F (CallTests, Call_FuncWhichContainsOnePlusOneExpression_ReturnsTwoAsResult) {
    auto codeOfFunc = "a = 1 + 1";
    auto call = "a";
    loli::Daphnie d{call};
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&_lexy);
    auto result =  (d.growTree()->visit(&_lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ (2.0f, result);
    loli::mem::Environment::Instance().Clear();
}


TEST_F (CallTests, Call_WithRecurtionCall_ReturnsFive) {
    auto codeOfFunc = "add a b => if (a < b) (add (a + b) (a + b)) else a";
    auto call = "add 1 4";
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&_lexy);
    auto result =  (
            loli::Daphnie{call}.growTree()->visit(&_lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ(5.0f,result);
    loli::mem::Environment::Instance().Clear();
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
    loli::mem::Environment::Instance().Clear();
}
#endif // __LOLI_CALL_TESTS__
