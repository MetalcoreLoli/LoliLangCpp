
#ifndef __TYPEMETHODS_TESTS__
#define __TYPEMETHODS_TESTS__


#include <cstddef>
#include <gmock/gmock-cardinalities.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest_pred_impl.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>

#include <loliLang/common.h>
#include "loliLang/expression.h"
#include "loliLang/lexy.h"
#include "loliLang/types.h"
#include "mockCommon.hpp"

class TypeMethodsTests : public ::testing::Test {
    protected:
        std::size_t _stdstringHashCode = typeid(std::string).hash_code();
        std::size_t _stdfloatHashCode = typeid(float).hash_code();
        std::size_t _stdboolHashCode = typeid(bool).hash_code();
};

/*
        virtual IMethod* GetMethodOfBoolType (const BoolType& type, std::string_view methodNameHashCode) = 0;
        virtual IMethod* GetMethodOfStringType (const StringType& type, std::string_view methodNameHashCode) = 0;
 * */
struct MockTypeMethodGetRequest : public loli::ITypeMethodGetRequest {
    MOCK_METHOD (loli::IMethod*, GetMethodOfBoolType, (const loli::BoolType& type, std::string_view methodNameHashCode), (override));
    MOCK_METHOD (loli::IMethod*, GetMethodOfFloatType, (loli::ITypeRequestHander*  type, std::string_view methodNameHashCode), (override));
    MOCK_METHOD (loli::IMethod*, GetMethodOfStringType, (const loli::StringType& type, std::string_view methodNameHashCode), (override));
};

/*
        virtual ReturnResult Invoke(const std::vector<ReturnResult>& args) = 0;
 * */
struct MockMethod : public loli::IMethod {
   MOCK_METHOD (loli::ReturnResult, Invoke, (const std::vector<loli::ReturnResult>& args), (override)); 
};


TEST_F (TypeMethodsTests, FloatType_GetMethod_WithAMethodHashCode_ReturnsPtrToMethod) {
    auto type = loli::FloatType();
    auto dummyGetter = MockTypeMethodGetRequest();

    EXPECT_CALL(dummyGetter,GetMethodOfFloatType(testing::_, testing::_)).Times(1);
    ON_CALL(dummyGetter, GetMethodOfFloatType)
        .WillByDefault([](auto t, auto c) { return new MockMethod();});

    //act 
    auto method = type.GetMethod(&dummyGetter, "");
    
    //assert 
    ASSERT_NE(method, nullptr);
}

TEST_F (TypeMethodsTests, BoolType_GetMethod_WithAMethodHashCode_ReturnsPtrToMethod) {
    auto type = loli::BoolType();
    auto dummyGetter = MockTypeMethodGetRequest();

    EXPECT_CALL(dummyGetter,GetMethodOfBoolType(testing::_, testing::_)).Times(1);
    ON_CALL(dummyGetter, GetMethodOfBoolType)
        .WillByDefault([](const loli::BoolType& t, auto c) { return new MockMethod();});

    //act 
    auto method = type.GetMethod(&dummyGetter, "");
    
    //assert 
    ASSERT_NE(method, nullptr);
}

TEST_F (TypeMethodsTests, StringType_GetMethod_WithAMethodHashCode_ReturnsPtrToMethod) {
    auto type = loli::StringType();
    auto dummyGetter = MockTypeMethodGetRequest();

    EXPECT_CALL(dummyGetter,GetMethodOfStringType(testing::_, testing::_)).Times(1);
    ON_CALL(dummyGetter, GetMethodOfStringType)
        .WillByDefault([](const loli::StringType& t, auto c) { return new MockMethod();});

    //act 
    auto method = type.GetMethod(&dummyGetter, "");
    
    //assert 
    ASSERT_NE(method, nullptr);
}

TEST_F (TypeMethodsTests, StringType_GetMethod_WithAMethodHashCode_AfterInvoke_ReturnsHelloWorld) {
    auto type = loli::StringType();
    auto dummyGetter = MockTypeMethodGetRequest();
    auto dummyMethod = MockMethod();

    EXPECT_CALL(dummyGetter,GetMethodOfStringType(testing::_, testing::_)).Times(1);
    ON_CALL(dummyGetter, GetMethodOfStringType)
        .WillByDefault([&dummyMethod](const loli::StringType& t, auto c) { return &dummyMethod;});

    EXPECT_CALL(dummyMethod, Invoke(testing::_)).Times(1);
    ON_CALL (dummyMethod, Invoke)
        .WillByDefault(
                [](const std::vector<loli::ReturnResult>& a) -> loli::ReturnResult { 
                    return loli::ReturnResult::New<std::string>("Hello, World"); 
                });
    //act 
    auto result = type.GetMethod(&dummyGetter, "")->Invoke({});
    
    //assert 
    ASSERT_EQ(_stdstringHashCode, result.TypeHashCode());
    ASSERT_STREQ(result.Unwrap<std::string>().c_str(), "Hello, World");
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsSum) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "+")->Invoke({
                loli::ReturnResult::New<float>(1.0f),
                loli::ReturnResult::New<float>(2.0f),
            });

    //assert
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(3.0f, result.Unwrap<float>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsSub) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "-")->Invoke({
                loli::ReturnResult::New<float>(2.0f),
                loli::ReturnResult::New<float>(1.0f),
            });

    //assert
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(1.0f, result.Unwrap<float>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsMul) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "*")->Invoke({
                loli::ReturnResult::New<float>(2.0f),
                loli::ReturnResult::New<float>(1.0f),
            });

    //assert
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(2.0f, result.Unwrap<float>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsDiv) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "/")->Invoke({
                loli::ReturnResult::New<float>(6.0f),
                loli::ReturnResult::New<float>(2.0f),
            });

    //assert
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ(3.0f, result.Unwrap<float>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsGt) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, ">")->Invoke({
                loli::ReturnResult::New<float>(6.0f),
                loli::ReturnResult::New<float>(2.0f),
            });

    //assert
    ASSERT_EQ(_stdboolHashCode, result.TypeHashCode());
    ASSERT_EQ(true, result.Unwrap<bool>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsLt) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "<")->Invoke({
                loli::ReturnResult::New<float>(2.0f),
                loli::ReturnResult::New<float>(6.0f),
            });

    //assert
    ASSERT_EQ(_stdboolHashCode, result.TypeHashCode());
    ASSERT_EQ(true, result.Unwrap<bool>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsNotEq) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "!=")->Invoke({
                loli::ReturnResult::New<float>(2.0f),
                loli::ReturnResult::New<float>(6.0f),
            });

    //assert
    ASSERT_EQ(_stdboolHashCode, result.TypeHashCode());
    ASSERT_EQ(true, result.Unwrap<bool>());
}

TEST_F (TypeMethodsTests, FloatType_GetMethod_With2Floats_ReturnsEq) {
    auto type = loli::FloatType();
    auto getter = loli::TypeMethodGetRequest();

    //act 
    auto result = type.GetMethod(&getter, "==")->Invoke({
                loli::ReturnResult::New<float>(2.0f),
                loli::ReturnResult::New<float>(6.0f),
            });

    //assert
    ASSERT_EQ(_stdboolHashCode, result.TypeHashCode());
    ASSERT_EQ(false, result.Unwrap<bool>());
}


TEST_F (TypeMethodsTests, AddMethod_With2Floats_ReturnsSum) {
    auto expression  = loli::BinaryExpression("+", new loli::NumberExpression(1), new loli::NumberExpression(2));
    auto typeChecker = loli::TypeChecker();
    auto getter = loli::TypeMethodGetRequest();
    
    //act 
    auto addMethod= expression.visit(&typeChecker).AsRawPtrOf<loli::IType>()->GetMethod(&getter, expression.operand());
    auto result = addMethod->Invoke({
                loli::ReturnResult::New(1.0f),
                loli::ReturnResult::New(2.0f),
            });

    //assert 
    ASSERT_EQ(_stdfloatHashCode, result.TypeHashCode());
    ASSERT_FLOAT_EQ (3.0f, result.Unwrap<float>());
}
#endif //__TYPEMETHODS_TESTS__
