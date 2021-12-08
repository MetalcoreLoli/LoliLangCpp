#include "lexy.h"

std::shared_ptr<void> loli::Lexy::visitBinaryExpression (loli::BinaryExpression& value) {
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

std::shared_ptr<void> loli::Lexy::visitNumberExpression (loli::NumberExpression& value) {
    return std::make_shared<float>(value.value());
}

std::shared_ptr<void> loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    return nullptr;
}

std::shared_ptr<void> loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    return nullptr;
}

std::shared_ptr<void> loli::Lexy::visitStringExpression (loli::StringExpression& value) {
    return nullptr;
}
