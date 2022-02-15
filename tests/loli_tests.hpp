#ifndef __LOLI_INTER_TESTS__
#define __LOLI_INTER_TESTS__

#include <cmath>
#include <exception>
#include <stdexcept>
#include <gtest/gtest.h>
#include <loliLang/common.h>


class LoliTests : public ::testing::Test {
    protected:
        loli::LoliInter _loli{};

};

TEST_F (LoliTests, AnswerOn_WithValidExpressionOnePlusOneExpression_ReturnsTwo) {

    //act 
    auto result = _loli.AnswerOn("1 + 1").As<float>();

    //assert
    ASSERT_GT(0.5f, std::abs(2.0f - result));
}

TEST_F (LoliTests, AnswerOn_WithCodeBlock_ReturnsFiftyFive) {
    auto code = R"(
        fibHelper a b n => 
            if (0 < n) fibHelper (a + b) a (n - 1) else a; 

        fib n = fibHelper 0 1 n;
        
        fib 10;
    )";
    //act 
    auto result = _loli.AnswerOn(code).As<float>();

    //assert
    ASSERT_FLOAT_EQ(result, 55.0f);
}
#endif // __LOLI_INTER__
