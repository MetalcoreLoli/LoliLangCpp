#ifndef __LEXY_TESTS__
#define __LEXY_TESTS__

#include "loliLang/expression.h"
#include <cmath>
#include <exception>
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
    auto result =  (loli::Daphnie{code}.growTree()->visit(&_lex)).Unwrap<bool>();

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
#endif //__LEXY_TESTS__
