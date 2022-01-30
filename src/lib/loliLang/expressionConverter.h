#ifndef __LOLI_EXPRESSION_CONVERTER_H__
#define __LOLI_EXPRESSION_CONVERTER_H__
#include "expression.h"
#include "loliLang/utils.h"

namespace loli {
    struct ExpressionConverter: IVisitor {
        GenericLink visitBinaryExpression (BinaryExpression& value) override;
        GenericLink visitNumberExpression (NumberExpression& value) override;
        GenericLink visitLambdaExpression (LambdaExpression& value) override;
        GenericLink visitIdentifierExpression (IdentifierExpression& value) override;
        GenericLink visitStringExpression (StringExpression& value) override;
        GenericLink visitIfExpression (IfExpression& value) override;
        GenericLink visitGroupingExpression (GroupingExpression& value) override;
        GenericLink visitBoolExpression (BoolExpression& value) override;
        GenericLink visitUnaryExpression (UnaryExpression& value) override;
        GenericLink visitClassExpression (ClassExpression& value) override;
        GenericLink visitBodyExpression (BodyExpression& value) override;
        GenericLink visitForExpression (ForExpression& value) override;
        GenericLink visitCallExpression (CallExpression& value) override;
        
        explicit ExpressionConverter (IVisitor* evaluator) : _evaluator (evaluator){}
        private:
            IVisitor* _evaluator = nullptr;
    };
};

#endif // __LOLI_EXPRESSION_CONVERTER_H__
