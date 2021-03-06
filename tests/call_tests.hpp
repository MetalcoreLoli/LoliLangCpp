#ifndef __LOLI_CALL_TESTS__
#define __LOLI_CALL_TESTS__

#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <loliLang/common.h>

#include "loliLang/daphnie.h"
#include "loliLang/expression.h"
#include "loliLang/expressionFactory.hpp"
#include "mockCommon.hpp"

class CallTests : public ::testing::Test {
    protected:
        loli::Lexy _lexy{};
};

TEST_F(CallTests, Call_FunctionWithWhere) {
    auto env  = loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto fib = "fib n => helper 0 1 n where helper a b n => if (0 < n) helper (a + b) a (n - 1) else a";

    //act 
    loli::Daphnie {fib}.growTree()->visit(&lexy);
    auto result = 
        LOLI_CALLFUNC_WA("fib", LOLI_NUMPTR(10))
            ->visit(&lexy); 

    //assert 
    ASSERT_EQ(result.TypeHashCode(),typeid(float).hash_code());
    ASSERT_FLOAT_EQ(55.0f, result.Unwrap<float>()); 
}

TEST_F (CallTests, Call_FibNumberTenWithRecursiveHelperFunctionCall_ReturnsFityFive) {
    auto env  = loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto fibHelper  = "fibHelper a b n => if (0 < n) fibHelper (a + b) a (n - 1) else a";
    auto fib        = "fib n => fibHelper 0 1 n";

    //act 
    loli::Daphnie {fibHelper}
        .growTree()
            ->visit(&lexy);
    loli::Daphnie {fib}.growTree()->visit(&lexy);

    auto result = loli::Daphnie {"fib 10"}.growTree()->visit(&lexy).Unwrap<float>();

    //assert 
    ASSERT_EQ (55.0f, result);
}

TEST_F (CallTests, Call_With3Args_ReturnsRigthValue) {
    auto env  = loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto a = loli::IdentifierExpression("a");
    auto b = loli::IdentifierExpression("b");
    auto c = loli::IdentifierExpression("c");
    auto body = loli::IfExpression(
            new loli::BinaryExpression("==", &a, new loli::NumberExpression(1)), 
            &a, 
            new loli::IfExpression(new loli::BinaryExpression("==", &b, new loli::NumberExpression(2)), &b, &c)
            );
    auto id = loli::ExpressionFactory::LambdaRaw("id", {c, b, a}, &body);

    //act
    id->visit(&lexy);
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
    auto env  = loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto a = LOLI_IDN("a");

    auto name = LOLI_IDN("id");
    auto id = loli::ExpressionFactory::LambdaRaw("id", {a}, LOLI_OPPTR("+", &a, LOLI_NUMPTR(1)));
    //act 
    id->visit(&lexy);
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
    return;
    auto env  = loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto codeOfFunc = "add a b => if (a < b) add (a + b) (a + b) else a";
    auto call = "add 1 4";
    loli::Daphnie dd{codeOfFunc};

    //act
    dd.growTree()->visit(&lexy);
    auto result =  (
            loli::Daphnie{call}.growTree()->visit(&lexy)).Unwrap<float>();

    //assert 
    ASSERT_EQ(5.0f,result);
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
