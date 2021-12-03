#ifndef __LEXY_H__
#define __LEXY_H__

#include "expression.h"

namespace loli {
    struct Lexy : IVisitor {
        std::shared_ptr<void> visitBinaryExpression (BinaryExpression& value) override;
        std::shared_ptr<void> visitNumberExpression (NumberExpression& value) override;
    };
};
#endif // __LEXY_H__
