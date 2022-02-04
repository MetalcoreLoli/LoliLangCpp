#include "lexy.h"
#include "loliLang/call.h"
#include "loliLang/expression.h"
#include "loliLang/expressionConverter.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/memory.h"
#include "loliLang/utils.h"

loli::ReturnResult loli::Lexy::visitBinaryExpression (loli::BinaryExpression& value) {
    if (_opsTable.contains(value.operand())) {
        auto leftValue  = (value.left()->visit(this).Unwrap<float>());
        auto rightValue = (value.right()->visit(this).Unwrap<float>());
        return _opsTable[value.operand()](leftValue, rightValue);
    } 
    throw std::runtime_error {"operator `"+value.operand()+"` is not implemented for type `Number`"};
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
    if (mem::Environment::Instance().TryFind(spec, nullptr)) {
        throw std::runtime_error {"There is another identifier with name `"+value.identifier().value()+"`"};
    }
    mem::Environment::Instance().Push(ExpressionFactory::LambdaRaw(value.identifier().value(), value.args(), value.body()));
    return l;
}

loli::ReturnResult loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    Expression* out = nullptr; 
    auto spec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.value());
    if (!mem::Environment::Instance().TryFind(spec, &out) && !_memory.TryFind(spec.get(), &out)) {
        throw std::runtime_error {"There is no `"+value.value()+"` identifier"};
    }
    auto func = dynamic_cast<LambdaExpression*>(out);
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
    _memory.Push(expression);
    return *this;
}

loli::ReturnResult loli::Lexy::visitCallExpression (loli::CallExpression& value) {
    return Call::Create().Validate(value).Map().FillLocalStackFrame(*this).Execute();  
}
