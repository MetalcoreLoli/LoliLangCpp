
#include "expressionConverter.h"
#include "loliLang/expression.h"
#include "loliLang/utils.h"

loli::GenericLink loli::ExpressionConverter::visitBinaryExpression (BinaryExpression& value) { 
    auto val = loli::unwrap<void, float> (value.visit(_evaluator));
    return utils::newLink<NumberExpression> (val);
}

loli::GenericLink loli::ExpressionConverter::visitNumberExpression (NumberExpression& value) { 
    auto val = loli::unwrap<void, float> (value.visit(_evaluator));
    return utils::newLink<NumberExpression> (val);
}

loli::GenericLink loli::ExpressionConverter::visitLambdaExpression (LambdaExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitLambdaExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitIdentifierExpression (IdentifierExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitIdentifierExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitStringExpression (StringExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitStringExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitIfExpression (IfExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitIfExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitGroupingExpression (GroupingExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitGroupingExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitBoolExpression (BoolExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitBoolExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitUnaryExpression (UnaryExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitUnaryExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitClassExpression (ClassExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitClassExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitBodyExpression (BodyExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitBodyExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitForExpression (ForExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitForExpression"); return nullptr; 
}

loli::GenericLink loli::ExpressionConverter::visitCallExpression (CallExpression& value) { 
    utils::ThrowHelper::Throw_NotImplemented("loli::ExpressionConverter::visitCallExpression"); return nullptr; 
}

