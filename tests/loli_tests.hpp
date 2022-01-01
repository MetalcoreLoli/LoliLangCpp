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
    auto result = loli::unwrap<void, float>(_loli.AnswerOn("1 + 1"));

    //assert
    ASSERT_GT(0.5f, std::abs(2.0f - result));
}
#endif // __LOLI_INTER__
