#ifndef __LOLI_CALL_TESTS__
#define __LOLI_CALL_TESTS__

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <loliLang/common.h>

#include "loliLang/expression.h"
#include "loliLang/expressionFactory.hpp"
#include "mockCommon.hpp"

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
    auto env  = MockEnvironment(); auto lexy = loli::Lexy(&env);

    EXPECT_CALL(env, TryFind(testing::_,testing::_)).Times(3);
    ON_CALL(env, TryFind)
        .WillByDefault([](auto s, loli::Expression **out) {
                auto a = loli::IdentifierExpression("a");
                auto b = loli::IdentifierExpression("b");
                auto c = loli::IdentifierExpression("c"); 
                auto zero = loli::NumberExpression(0);
                auto aghtzero = loli::BinaryExpression("!=", &a, &zero);
                auto bghtzero = loli::BinaryExpression("!=", &b, &zero);
                *out = loli::ExpressionFactory::LambdaRaw(
                        "id", 
                        {c,b,a},
                        new loli::IfExpression(
                            &aghtzero, &a, new loli::IfExpression(&bghtzero, &b, &c)));
                 return true;
                });

    //act
    auto resultA = 
         (loli::ExpressionFactory::Call("id", {loli::ExpressionFactory::NumberRaw(1), loli::ExpressionFactory::NumberRaw(2), loli::ExpressionFactory::NumberRaw(3)}).get()->visit(&lexy)).Unwrap<float>();
    auto resultB = 
         (loli::ExpressionFactory::Call("id", {loli::ExpressionFactory::NumberRaw(0), loli::ExpressionFactory::NumberRaw(2), loli::ExpressionFactory::NumberRaw(3)}).get()->visit(&lexy)).Unwrap<float>();
    auto resultC = 
         (loli::ExpressionFactory::Call("id", {loli::ExpressionFactory::NumberRaw(0), loli::ExpressionFactory::NumberRaw(0), loli::ExpressionFactory::NumberRaw(3)}).get()->visit(&lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ(1.0f,resultA);
    ASSERT_EQ(2.0f,resultB);
    ASSERT_EQ(3.0f,resultC);
}


TEST_F (CallTests, Call_FuncWithOneArgInWhichPassedOneUndUsedInsideFuncsBody_ReturnsTen) {
    auto env  = MockEnvironment(); auto lexy = loli::Lexy(&env);

    EXPECT_CALL(env, TryFind(testing::_,testing::_)).Times(1);
    ON_CALL(env, TryFind)
        .WillByDefault([](auto s, loli::Expression **out) {
                *out = loli::ExpressionFactory::LambdaRaw(
                        "id", 
                        {loli::IdentifierExpression("a")}, 
                        new loli::BinaryExpression(
                            "+", 
                            new loli::IdentifierExpression("a"), new loli::NumberExpression(1)));
                 return true;
                });

    auto name = loli::IdentifierExpression("id");
    //act 
    auto result = loli::CallExpression(name, {loli::ExpressionFactory::NumberRaw(9)}).visit(&lexy).Unwrap<float>();

    //assert 
    ASSERT_EQ (10.0f, result);
}

TEST_F (CallTests, Call_FuncWhichContainsOnePlusOneExpression_ReturnsTwoAsResult) {
    auto env  = MockEnvironment(); auto lexy = loli::Lexy(&env);
    auto func = loli::ExpressionFactory::LambdaRaw(
            "a", 
            new loli::BinaryExpression("+", new loli::NumberExpression(1), new loli::NumberExpression(1)));

    auto call = *loli::ExpressionFactory::CallWithoutArgs("a");

    EXPECT_CALL(env, TryFind(::testing::_, ::testing::_)).Times(1);
    ON_CALL(env, TryFind)
        .WillByDefault([&func](auto spec, loli::Expression** out){
                *out = func;
                return true;
            });
    //act
    auto result =  (call.visit(&lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ (2.0f, result);
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
    auto env  = MockEnvironment(); auto lexy = loli::Lexy(&env);

    auto func = loli::ExpressionFactory::LambdaRaw(
            "add", {
                loli::IdentifierExpression("a"),
                loli::IdentifierExpression("b")
            }, 
            new loli::BinaryExpression("+", 
                new loli::IdentifierExpression("a"), new loli::IdentifierExpression("b")));;

    auto call = *loli::ExpressionFactory::Call("add", {
                new loli::NumberExpression(1),
                new loli::NumberExpression(2),
                new loli::NumberExpression(3)
            }).get();

    EXPECT_CALL(env, TryFind(::testing::_, ::testing::_)).Times(3);
    ON_CALL(env, TryFind)
        .WillByDefault([&func](auto spec, loli::Expression** out){
                *out = func;
                return true;
            });
    //act
    auto act = [](loli::CallExpression call, loli::Lexy lexy) {call.visit(&lexy);};

    //assert 
    ASSERT_THROW (act(call, lexy), std::runtime_error);
    try {
        act (call, lexy);
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "There is a extra arg in call of `add` function");
    }
    try {
        act (*loli::ExpressionFactory::Call("add",{new loli::NumberExpression(1)}).get(), lexy);
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "There is a missing arg in call of `add` function");
    }
}
#endif // __LOLI_CALL_TESTS__
