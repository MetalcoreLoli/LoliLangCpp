
#include "expressionConverter.h"
#include "loliLang/expression.h"
#include "loliLang/utils.h"

loli::ReturnResult loli::ExpressionConverter::visitBinaryExpression (BinaryExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::BinaryExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitNumberExpression (NumberExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::NumberExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitLambdaExpression (LambdaExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitLambdaExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitIdentifierExpression (IdentifierExpression& value) { 
    return value.visit(_evaluator);
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitIdentifierExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitStringExpression (StringExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitStringExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitIfExpression (IfExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitIfExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitGroupingExpression (GroupingExpression& value) { 
    return value.expression()->visit(this);
    //utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitGroupingExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitBoolExpression (BoolExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitBoolExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitUnaryExpression (UnaryExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitUnaryExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitClassExpression (ClassExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitClassExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitBodyExpression (BodyExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitBodyExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitForExpression (ForExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitForExpression"); return {nullptr, 0}; 
}

loli::ReturnResult loli::ExpressionConverter::visitCallExpression (CallExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitCallExpression"); return {nullptr, 0}; 
}

