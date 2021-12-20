#ifndef __LEXY_H__
#define __LEXY_H__

#include "expression.h"

namespace loli {
    struct Lexy : IVisitor {
        loli::GenericLink visitBinaryExpression (BinaryExpression& value) override;
        loli::GenericLink visitNumberExpression (NumberExpression& value) override;
        loli::GenericLink visitLambdaExpression (LambdaExpression& value)  override;
        loli::GenericLink visitIdentifierExpression (IdentifierExpression& value) override;
        loli::GenericLink visitStringExpression (StringExpression& value) override;
        loli::GenericLink visitIfExpression (IfExpression& value) override;
        loli::GenericLink visitGroupingExpression (GroupingExpression& value) override;
        loli::GenericLink visitBoolExpression (BoolExpression& value) override;
        loli::GenericLink visitUnaryExpression (UnaryExpression& value) override;
    };
};
#endif // __LEXY_H__
