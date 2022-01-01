#include "lexy.h"

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
    throw std::runtime_error {"The Lexy::visitUnaryExpression is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitLambdaExpression (loli::LambdaExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitIdentifierExpression (loli::IdentifierExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitStringExpression (loli::StringExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitIfExpression(loli::IfExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitGroupingExpression (loli::GroupingExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitBoolExpression (loli::BoolExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitForExpression(loli::ForExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitClassExpression (loli::ClassExpression & value) {
    throw std::runtime_error{"the method is not implemented yet."};
}

loli::GenericLink loli::Lexy::visitBodyExpression (loli::BodyExpression& value) {
    throw std::runtime_error{"the method is not implemented yet."};
}
