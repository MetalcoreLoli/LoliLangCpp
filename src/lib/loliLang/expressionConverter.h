#ifndef __LOLI_EXPRESSION_CONVERTER_H__
#define __LOLI_EXPRESSION_CONVERTER_H__
#include "expression.h"
#include "loliLang/utils.h"

namespace loli {
    struct ExpressionConverter: IVisitor {
        ReturnResult visitBinaryExpression (BinaryExpression& value) override;
        ReturnResult visitNumberExpression (NumberExpression& value) override;
        ReturnResult visitLambdaExpression (LambdaExpression& value) override;
        ReturnResult visitIdentifierExpression (IdentifierExpression& value) override;
        ReturnResult visitStringExpression (StringExpression& value) override;
        ReturnResult visitIfExpression (IfExpression& value) override;
        ReturnResult visitGroupingExpression (GroupingExpression& value) override;
        ReturnResult visitBoolExpression (BoolExpression& value) override;
        ReturnResult visitUnaryExpression (UnaryExpression& value) override;
        ReturnResult visitClassExpression (ClassExpression& value) override;
        ReturnResult visitBodyExpression (BodyExpression& value) override;
        ReturnResult visitForExpression (ForExpression& value) override;
        ReturnResult visitCallExpression (CallExpression& value) override;
        
        explicit ExpressionConverter (IVisitor* evaluator) : _evaluator (evaluator){}
        private:
            IVisitor* _evaluator = nullptr;
    };
};

#endif // __LOLI_EXPRESSION_CONVERTER_H__
