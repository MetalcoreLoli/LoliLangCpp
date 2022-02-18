#ifndef __LEXY_H__
#define __LEXY_H__

#include <map>
#include <string>
#include <functional>


#include "expression.h"
#include "loliLang/memory.h"
#include "loliLang/utils.h"
#include "expressionFactory.hpp"

namespace loli {
    struct Lexy : IVisitor {
        ReturnResult visitBinaryExpression (BinaryExpression& value) override;
        ReturnResult visitNumberExpression (NumberExpression& value) override;
        ReturnResult visitLambdaExpression (LambdaExpression& value) override;
        ReturnResult visitIdentifierExpression (IdentifierExpression& value) override;
        ReturnResult visitStringExpression (StringExpression& value) override;
        ReturnResult visitIfExpression (IfExpression& value) override;
        ReturnResult visitGroupingExpression (GroupingExpression& value) override;
        ReturnResult visitBoolExpression (BoolExpression& value) override;
        ReturnResult visitUnaryExpression (UnaryExpression& value) override;
        ReturnResult visitClassExpression (ClassExpression& value) override;
        ReturnResult visitBodyExpression (BodyExpression& value) override;
        ReturnResult visitForExpression (ForExpression& value) override;
        ReturnResult visitCallExpression (CallExpression& value) override;
        ReturnResult visitWhereExpression (WhereExpression& value) override;

        Lexy& PushIntoMainStack (Expression* expression);

        explicit Lexy () : _globalEnv(&mem::GlobalEnvironment){
        }

        Lexy(mem::IEnvironment* env) : _globalEnv(env) {}
        private:
            mem::IEnvironment* _globalEnv{};
            mem::LocalEnvironment _localEnv{};
    };
};
#endif // __LEXY_H__
