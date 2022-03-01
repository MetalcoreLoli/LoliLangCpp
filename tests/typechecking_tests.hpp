#ifndef __TYPECHECKING_TESTS__
#define __TYPECHECKING_TESTS__


#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest_pred_impl.h>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>

#include <loliLang/common.h>
#include "loliLang/expressionFactory.hpp"
#include "loliLang/types.h"
#include "mockCommon.hpp"


class TypecheckingTests : public ::testing::Test{
    protected:
        size_t _floatTypeHashCode = typeid(loli::FloatType).hash_code();
        size_t _stringTypeHashCode = typeid(loli::StringType).hash_code();
        size_t _boolTypeHashCode = typeid(loli::BoolType).hash_code();
        MockEnvironment _mockEnv {};
        loli::Lexy _lexy{};
};


TEST_F(TypecheckingTests, NumberExpression_ShouldReturnFloatType) {
    auto num = loli::NumberExpression(0.f);
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(_floatTypeHashCode, type.TypeHashCode());
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
    ASSERT_EQ(_stringTypeHashCode, type.TypeHashCode());
}

TEST_F(TypecheckingTests, BinaryExpression_With2Floats_ReturnsFloatType) {
    auto num = loli::BinaryExpression("+", new loli::NumberExpression(0), new loli::NumberExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(_floatTypeHashCode, type.TypeHashCode());
}

TEST_F(TypecheckingTests, BinaryExpression_With2Boolens_ReturnsBoolType) {
    auto num = loli::BinaryExpression("+", new loli::BoolExpression(0), new loli::BoolExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = num.visit(&typeChecker); 

    //assert 
    ASSERT_EQ(_boolTypeHashCode, type.TypeHashCode());
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
    ASSERT_EQ(_floatTypeHashCode, type.TypeHashCode());
}

TEST_F (TypecheckingTests, IfExpression_WithWrongTypeCondition_ThrowsRuntimeError) {
    auto num = loli::IfExpression(new loli::BinaryExpression("+",  new loli::NumberExpression(0), new loli::NumberExpression(0)), new loli::NumberExpression(0), new loli::NumberExpression(0));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = [](loli::IfExpression n, loli::TypeChecker typeChecker) {n.visit(&typeChecker);}; 

    //assert 
    ASSERT_THROW(type(num, typeChecker), std::runtime_error);
    try {
        type(num, typeChecker);
    } catch (const std::runtime_error& ex) {
        ASSERT_STREQ(ex.what(), "There should be boolen expression inside of a condition block");
    }
}

TEST_F (TypecheckingTests, LambdaExpression_WithFloatNumberInsideBody_ReturnsFloatType){
    auto ll = *loli::ExpressionFactory::Lambda("a", new loli::NumberExpression(1));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = ll.visit(&typeChecker);

    //assert 
    ASSERT_EQ(_floatTypeHashCode, type.TypeHashCode());

}

TEST_F (TypecheckingTests, LambdaExpression_WithTrueInsideBody_ReturnsFloatType){
    auto ll = *loli::ExpressionFactory::Lambda("a", new loli::BoolExpression(1));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = ll.visit(&typeChecker);

    //assert 
    ASSERT_EQ(_boolTypeHashCode, type.TypeHashCode());

}

TEST_F (TypecheckingTests, LambdaExpression_WithStringInsideBody_ReturnsFloatType){
    auto ll = *loli::ExpressionFactory::Lambda("a", new loli::StringExpression("hello"));
    auto typeChecker = loli::TypeChecker();

    //act 
    auto type = ll.visit(&typeChecker);

    //assert 
    ASSERT_EQ(_stringTypeHashCode, type.TypeHashCode());
}

TEST_F (TypecheckingTests, IdentifierExpression_WithFloatValue_ReturnsFloatType) {
    using ::testing::_;
    using::testing::WithArg;
    auto typeChecker = loli::TypeChecker(&_mockEnv);
    auto a = loli::ExpressionFactory::LambdaRaw("a", new loli::NumberExpression(1));

    EXPECT_CALL(_mockEnv,TryFind(_, _)).Times(1);
    ON_CALL(_mockEnv, TryFind)
        .WillByDefault([&a](const loli::mem::ExpressionFilter spec, loli::Expression **out) {
                *out = a;
                return true;
            });
    
    //act 
    auto type = loli::IdentifierExpression("a").visit(&typeChecker);

    //assert 
    ASSERT_EQ(type.TypeHashCode(), _floatTypeHashCode);
}

TEST_F (TypecheckingTests, CallExpression_With2Floats_ReturnsFloatType) {
    auto mockEnv        = MockEnvironment();
    auto typeChecker    = loli::TypeChecker(&mockEnv);
    auto a = LOLI_FUNCAPTR ("a", LOLI_NUMPTR(1));

    EXPECT_CALL(mockEnv, TryFind(testing::_, testing::_)).Times(1);
    ON_CALL(mockEnv, TryFind)
        .WillByDefault([&a](const auto& spec, loli::Expression **out) {
                    *out = a;
                    return true;
                });
    //act 
    auto type = LOLI_CALLFUNC("a")->visit(&typeChecker);

    //assert 
    ASSERT_EQ(_floatTypeHashCode, type.TypeHashCode());
}
#endif //__TYPECHECKING_TESTS__
