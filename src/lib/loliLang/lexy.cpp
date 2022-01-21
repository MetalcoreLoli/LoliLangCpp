#include "lexy.h"
#include "loliLang/expression.h"
#include <algorithm>
#include <typeinfo>

loli::GenericLink loli::Lexy::visitBinaryExpression (loli::BinaryExpression& value) {
    if (_opsTable.contains(value.operand())) {
        auto leftValue  = *std::static_pointer_cast<float>(value.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<float>(value.right()->visit(this));
        return _opsTable[value.operand()](leftValue, rightValue);
    } else {
        throw std::runtime_error {"operator `"+value.operand()+"` is not implemented for type `Number`"};
    }
    return nullptr;
}

loli::GenericLink loli::Lexy::visitNumberExpression (loli::NumberExpression& value) {
    return std::make_shared<float>(value.value());
}

loli::GenericLink loli::Lexy::visitUnaryExpression(loli::UnaryExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitUnaryExpression");
}

loli::GenericLink loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    auto l = loli::newLink<LambdaExpression>(value);
    _memory->Push(l.get());
    return l;
}

loli::GenericLink loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    Expression* out = ExpressionFactory::EmptyLambdaExpression(); 
    if (!_memory->TryFind(ExpressionSpecFactory::LambdaExpressionTypeSpec().get(), &out)) {
        throw std::runtime_error {"There is no `"+value.value()+"` identifier"};
    }
    auto func = dynamic_cast<LambdaExpression*>(out);
    return func->body()->visit(this);
}

loli::GenericLink loli::Lexy::visitStringExpression (loli::StringExpression& value) {
    return loli::newLink<std::string> (value.value());
}

loli::GenericLink loli::Lexy::visitIfExpression(loli::IfExpression& value) {
    auto condition = loli::unwrap<void, bool> (value.condition()->visit(this));
    if (condition) {
        return value.then()->visit(this);
    } else {
        return value.els()->visit(this);
    }
}

loli::GenericLink loli::Lexy::visitGroupingExpression (loli::GroupingExpression& value) {
    return value.expression()->visit(this);
}

loli::GenericLink loli::Lexy::visitBoolExpression (loli::BoolExpression& value) {
    return loli::newLink<bool>(value.value());
}

loli::GenericLink loli::Lexy::visitForExpression(loli::ForExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitForExpression");
}

loli::GenericLink loli::Lexy::visitClassExpression (loli::ClassExpression & value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitClassExpression");
}

loli::GenericLink loli::Lexy::visitBodyExpression (loli::BodyExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitBodyExpression");
}

loli::Lexy& loli::Lexy::PushIntoMainStack (loli::Expression* expression) {
    _mainStack.push_back(expression);
    return *this;
}

loli::GenericLink loli::Lexy::visitCallExpression (loli::CallExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::CallExpression");
}
