#include "lexy.h"
#include "loliLang/expression.h"
#include "loliLang/expressionConverter.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/utils.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <typeinfo>

#ifdef LOLI_LOG_STD
#include <iostream>
#include "ast.cpp"
#endif 

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
}

loli::ReturnResult loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    ReturnResult l = {loli::newLink<LambdaExpression> (value), typeid(LambdaExpression).hash_code()};
    _memory.Push(ExpressionFactory::LambdaRaw(value.identifier().value(), value.args(), value.body()));
    return l;
}

loli::ReturnResult loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    Expression* out = nullptr; 
    auto spec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.value()).get();
    if (!_memory.TryFind(spec, &out)) {
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
    Expression* out = nullptr;
    auto nameSpec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.idetifier().value()).get();
    if (!_memory.TryFind(nameSpec, &out)) {
        utils::ThrowHelper::Throw_ThereIsNo(value.idetifier().value()); 
    }
    auto lambda = *(dynamic_cast <LambdaExpression*>(out));
    if (lambda.args().size() > value.args().size()) {
        throw std::runtime_error {
            "There is a missing arg in call of `"+value.idetifier().value()+"` function"};
    } else if (lambda.args().size() < value.args().size()) {
        throw std::runtime_error {
            "There is a extra arg in call of `"+value.idetifier().value()+"` function"};
    }

    std::vector<LambdaExpression*> as {};
    Lexy local{};
    ExpressionConverter con{&local};
    local.PushIntoMainStack(&lambda);
    for (size_t i = 0; i < lambda.args().size(); i++) {
        auto name = lambda.args()[i].value();
        auto arg = value.args()[lambda.args().size() - 1 - i];
        as.push_back(ExpressionFactory::LambdaRaw(name, arg));
    }
    for (auto a : as) {
        if (a->IsLiteral)
            local.PushIntoMainStack(a);
        else {
            auto result =  (a->body()->visit(this));
            local.PushIntoMainStack(ExpressionFactory::LambdaRaw(a->identifier().value(), ExpressionFactory::FromReturnResult(result)));
        }
    }
#ifdef LOLI_LOG_STD
    std::cout << "Dump: "<< lambda.identifier().value() << std::endl;
    loli::ASTAsString ast{};
    for (auto arg: as) {
        std::cout << (arg->visit(&ast)).Unwrap<std::string>() << std::endl;
    }
    std::cout <<  (lambda.visit(&ast)).Unwrap<std::string>() << std::endl;
#endif
    return lambda.body()->visit(&local);
}
