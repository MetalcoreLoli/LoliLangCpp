
#ifndef __TYPECHECKING_TESTS__
#define __TYPECHECKING_TESTS__

#include "loliLang/daphnie.h"
#include "loliLang/expression.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/lexy.h"
#include "loliLang/types.h"
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <loliLang/common.h>

class TypecheckingTests : public ::testing::Test{
    protected:
        loli::Lexy _lexy{};
};

TEST_F(TypecheckingTests, NumberExpression_ShouldReturnFloatType) {
    auto num = loli::NumberExpression(0.f);
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::FloatType).hash_code(), type.TypeHashCode());
}

TEST_F(TypecheckingTests, BoolExpression_ShouldReturnBool) {
    auto num = loli::BoolExpression(true);
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::BoolType).hash_code(), type.TypeHashCode());
}

TEST_F(TypecheckingTests, StringExpression_ShouldReturnStringType) {
    auto num = loli::StringExpression("hello");
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::StringType).hash_code(), type.TypeHashCode());
}

TEST_F(TypecheckingTests, BinaryExpression_With2Floats_ReturnsFloatType) {
    auto num = loli::BinaryExpression("+", new loli::NumberExpression(0), new loli::NumberExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::FloatType).hash_code(), type.TypeHashCode());
}

TEST_F(TypecheckingTests, BinaryExpression_With2Boolens_ReturnsBoolType) {
    auto num = loli::BinaryExpression("+", new loli::BoolExpression(0), new loli::BoolExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::BoolType).hash_code(), type.TypeHashCode());
}

TEST_F(TypecheckingTests, BinaryExpression_WithWrongTypes_ThrowsRuntimeError) {
    auto num = loli::BinaryExpression("+", new loli::BoolExpression(0), new loli::NumberExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto act = [](loli::BinaryExpression n, loli::TypeChecker typeChecker) {n.visit(&typeChecker); };

    //assert 
    ASSERT_THROW(act(num, typeChecker), std::runtime_error);
    try {
        act(num, typeChecker);
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "type of left value is not the same as type of right value");
    }
}

TEST_F (TypecheckingTests, IfExpression_With2Floats_ReturnsFloatType) {
    auto num = loli::IfExpression(new loli::BoolExpression(true), new loli::NumberExpression(0), new loli::NumberExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(typeid(loli::FloatType).hash_code(), type.TypeHashCode());
}
#endif //__TYPECHECKING_TESTS__
