#ifndef __LEXY_TESTS__
#define __LEXY_TESTS__

#include "loliLang/daphnie.h"
#include "loliLang/expression.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/lexy.h"
#include "loliLang/memory.h"
#include "mockCommon.hpp"
#include <cmath>
#include <exception>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <stack>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <loliLang/common.h>
#include <path_utils.h>

class LexyTests : public ::testing::Test {
    protected:
        loli::Lexy _lex{};
        const size_t _loliLambdaHashCode = typeid(loli::LambdaExpression).hash_code();
        const size_t _stdfloatHashCode = typeid(float).hash_code();
};

class MockCaller : public loli::ICaller {
    public:
        MOCK_METHOD(
                loli::ReturnResult, 
                callLambdaExpression, 
                (loli::LambdaExpression&, std::stack<loli::Expression*>&),
                (override));
};

TEST_F (LexyTests, visitBinaryExpression_WithOneAddOne_ReturnsTwo) {
    loli::Daphnie d{"1 + 1"};

    //act 
    auto result =  (d.growTree()->visit(&_lex)).Unwrap<float>();


    //assert 
    ASSERT_GT (0.05f, std::abs(2.0f - result));
}

TEST_F (LexyTests, visitIfExpression_WithOneSubOneLtOne_ReturnsTrue) {
    std::string code = "if ((1-1) < 1) true else false;";

    //act 
    auto tree = loli::Daphnie{code}.growTree(); 
    auto resultOfTreeEval = tree->visit(&_lex);
    auto result =  resultOfTreeEval.Unwrap<bool>();

    //assert
    ASSERT_TRUE(result);
}


TEST_F (LexyTests, visitLambdaExpression_WithValidOnePlusOne_ReturnsTwo) {
    using ::testing::Return;
    auto code = "add => 1 + 1;";
    MockCaller caller {};
    std::stack<loli::Expression*> _emptyStack{};
    EXPECT_CALL(caller,
                callLambdaExpression(testing::_, testing::_))
            .Times(1)
            .WillOnce(Return(loli::ReturnResult{loli::newLink<float>(2.0f), typeid(float).hash_code()}));

    //act
    auto func =
        (loli::Daphnie{code}.growTree()->visit(&_lex)).Unwrap<loli::LambdaExpression>();

    auto result = (func.call(&caller, _emptyStack)).Unwrap<float>();

    //assert
    ASSERT_EQ(2.0f, result);
}

TEST_F(LexyTests, visitLambdaExpression_WithCallExpressionWithoutArgs) {
    auto env =loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    loli::ExpressionFactory::LambdaRaw("b", new loli::NumberExpression(1))->visit(&lexy);

    auto l = 
        loli::ExpressionFactory::LambdaRaw("a", new loli::IdentifierExpression("b"));
    //act 
    l->visit(&lexy);
    auto result = (new loli::IdentifierExpression("a"))->visit(&lexy); 

    //assert 
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(1.0f, result.Unwrap<float>());
}

TEST_F(LexyTests, visitWhereExpression_WithLambdaExpression_EvalsIt) {
    auto env =loli::mem::LocalEnvironment(); auto lexy = loli::Lexy(&env);
    auto where = loli::WhereExpression(
            loli::ExpressionFactory::LambdaRaw("a", new loli::IdentifierExpression("b")), {
                loli::ExpressionFactory::LambdaRaw("b", new loli::NumberExpression(1))
            });
    
    //act
    where.visit(&lexy);
    auto result = 
        (new loli::IdentifierExpression("a"))
            ->visit(&lexy);

    // assert
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(1.0f, result.Unwrap<float>());
}
#endif //__LEXY_TESTS__
