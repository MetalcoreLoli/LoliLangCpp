#include "lexy.h"

loli::GenericLink loli::Lexy::visitBinaryExpression (loli::BinaryExpression& value) {
    if (value.operand() == "+") {
        auto leftValue  = *std::static_pointer_cast<float>(value.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<float>(value.right()->visit(this));
        return std::make_shared<float>(leftValue + rightValue);
    }
    if (value.operand() == "-") {
        auto leftValue  = *std::static_pointer_cast<float>(value.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<float>(value.right()->visit(this));
        return std::make_shared<float>(leftValue - rightValue);
    }
    if (value.operand() == "*") {
        auto leftValue  = *std::static_pointer_cast<float>(value.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<float>(value.right()->visit(this));
        return std::make_shared<float>(leftValue * rightValue);
    }
    if (value.operand() == "/") {
        auto leftValue  = *std::static_pointer_cast<float>(value.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<float>(value.right()->visit(this));
        return std::make_shared<float>(leftValue  / rightValue);
    }
    return nullptr;
}

loli::GenericLink loli::Lexy::visitNumberExpression (loli::NumberExpression& value) {
    return std::make_shared<float>(value.value());
}

loli::GenericLink loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    return nullptr;
}

loli::GenericLink loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    return nullptr;
}

loli::GenericLink loli::Lexy::visitStringExpression (loli::StringExpression& value) {
    return nullptr;
}

loli::GenericLink loli::Lexy::visitIfExpression(loli::IfExpression& value) {
    return nullptr;
}

loli::GenericLink loli::Lexy::visitGroupingExpression (loli::GroupingExpression& value) {
    return nullptr;
}
loli::GenericLink loli::Lexy::visitBoolExpression (loli::BoolExpression& value) {
    return nullptr;
}
