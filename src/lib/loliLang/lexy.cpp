#include "lexy.h"
#include "loliLang/call.h"
#include "loliLang/expression.h"
#include "loliLang/expressionConverter.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/memory.h"
#include "loliLang/types.h"
#include "loliLang/utils.h"
#include <vector>

loli::ReturnResult loli::Lexy::visitBinaryExpression (loli::BinaryExpression& value) {
    auto env = mem::Or(_globalEnv, _localEnv);
    auto method = TypeChecker::GetMethod<TypeMethodGetRequest>(&env, &value, value.operand());
    return method->Invoke({
                value.right()->visit(this),
                value.left()->visit(this),
            });
}

loli::ReturnResult loli::Lexy::visitNumberExpression (loli::NumberExpression& value) {
    return {loli::newLink<float>(value.value()), typeid(float).hash_code()};
}

loli::ReturnResult loli::Lexy::visitUnaryExpression(loli::UnaryExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitUnaryExpression");
    return ReturnResult::Empty();
}

loli::ReturnResult loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    ReturnResult l = {loli::newLink<LambdaExpression> (value), typeid(LambdaExpression).hash_code()};
    auto spec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.identifier().value());
    if (_globalEnv->TryFind(spec, nullptr)) {
        throw std::runtime_error {"There is another identifier with name `"+value.identifier().value()+"`"};
    }
    _globalEnv->Push(&value);
    return l;
}

loli::ReturnResult loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    Expression* out = nullptr; 
    auto spec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.value());
    auto env = mem::Or(_globalEnv, _localEnv);
    if (!env.TryFind(spec, &out)) {
        throw std::runtime_error {"There is no `"+value.value()+"` identifier1"};
    }
    auto func = dynamic_cast<LambdaExpression*>(out);
    if (func->hasWhereBlock()) {
        for (auto arg: func->where()) {
            PushIntoMainStack(arg);
        }
    }
    return func->body()->visit(this);
}

loli::ReturnResult loli::Lexy::visitStringExpression (loli::StringExpression& value) {
    return {loli::newLink<std::string> (value.value()), typeid(std::string).hash_code()};
}

loli::ReturnResult loli::Lexy::visitIfExpression(loli::IfExpression& value) {
    auto condition = value.condition()->visit(this).Unwrap<bool>();
    if (condition) {
        return value.then()->visit(this);
    } else {
        return value.els()->visit(this);
    }
}

loli::ReturnResult loli::Lexy::visitGroupingExpression (loli::GroupingExpression& value) {
    return value.expression()->visit(this);
}

loli::ReturnResult loli::Lexy::visitBoolExpression (loli::BoolExpression& value) {
    return {loli::newLink<bool>(value.value()), typeid(bool).hash_code()};
}

loli::ReturnResult loli::Lexy::visitForExpression(loli::ForExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitForExpression");
    return {nullptr, 0};
}

loli::ReturnResult loli::Lexy::visitClassExpression (loli::ClassExpression & value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitClassExpression");
    return {nullptr, 0};
}

loli::ReturnResult loli::Lexy::visitBodyExpression (loli::BodyExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitBodyExpression");
    return {nullptr, 0};
}

loli::Lexy& loli::Lexy::PushIntoMainStack (loli::Expression* expression) {
    _localEnv->Push(expression);
    return *this;
}

loli::ReturnResult loli::Lexy::visitCallExpression (loli::CallExpression& value) {
    auto env = mem::Or(_globalEnv, _localEnv); 
    return Call::Create(_globalEnv).Validate(value, _localEnv).Map().FillLocalStackFrame(*this).Execute();  
}

loli::ReturnResult loli::Lexy::visitWhereExpression (WhereExpression& value) {
    auto f = value.func();
    f->where(std::vector<Expression*>(value.args().begin(), value.args().end()));
    return f->visit(this);
}
