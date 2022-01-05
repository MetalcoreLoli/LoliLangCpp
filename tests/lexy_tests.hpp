#ifndef __LEXY_TESTS__
#define __LEXY_TESTS__

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

class MockCaller : public loli::ICallable {
    public:
        MOCK_METHOD(
                loli::GenericLink, 
                callLambdaExpression, 
                (loli::LambdaExpression&, std::stack<loli::Expression*>&),
                (override));
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

TEST_F (LexyTests, visitIfExpression_WithOneSubOneLtOne_ReturnsTrue) {
    std::string code = "if ((1-1) < 1) true else false;";

    //act 
    auto result = loli::unwrap<void, bool> (loli::Daphnie{code}.growTree()->visit(&_lex));

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
            .WillOnce(Return(loli::newLink<float>(2.0f)));

    //act
    auto func =
        loli::unwrap<void, loli::LambdaExpression>(loli::Daphnie{code}.growTree()->visit(&_lex));

    auto result = loli::unwrap<void, float>(func.call(&caller, _emptyStack));

    //assert
    ASSERT_EQ(2.0f, result);
}
#endif //__LEXY_TESTS__
