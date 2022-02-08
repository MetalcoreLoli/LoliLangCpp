#ifndef __LOLI_MOCK_COMMON__
#define __LOLI_MOCK_COMMON__
#include <gmock/gmock-actions.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>

#include <loliLang/common.h>
#include "loliLang/memory.h"

/*
 * 
            virtual IEnvironment& Push (Expression*) =0;
            virtual IEnvironment& Clear() =0;
            virtual bool TryFind (const ExpressionFilter spec, Expression **out) = 0;
 *
 * */
class MockEnvironment : public loli::mem::IEnvironment {
    public:
    MOCK_METHOD(IEnvironment&, Push, (loli::Expression*), (override));
    MOCK_METHOD(IEnvironment&, Clear, (), (override));
    MOCK_METHOD(bool, TryFind, (const loli::mem::ExpressionFilter spec, loli::Expression **out), (override));
};
#endif // __LOLI_MOCK_COMMON__
