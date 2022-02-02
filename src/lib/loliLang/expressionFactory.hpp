#ifndef __LOLI_EXPRESSION_FACTORY_H__
#define __LOLI_EXPRESSION_FACTORY_H__

#include "expression.h"
#include "utils.h"

namespace loli {
    using namespace utils;
    struct ExpressionFactory {
        static LambdaExpression* EmptyLambdaExpression () {
            IdentifierExpression empty("EMPTY");
            return new class LambdaExpression (empty, nullptr);
        }

        static Link<class LambdaExpression> Lambda(const std::string& name, Expression* body) {
            IdentifierExpression n(name);
            return newLink<class LambdaExpression> (n, body);
        }
        static class LambdaExpression* LambdaRaw(const std::string& name, Expression* body) {
            IdentifierExpression n(name);
            return  new class LambdaExpression(n, body);
        }
        static LambdaExpression* LambdaRaw(const std::string& name, const std::vector<loli::IdentifierExpression>& args, Expression* body) {
            IdentifierExpression n(name);
            return new LambdaExpression(n, args, body);
        }

        static Link<class CallExpression>  CallWithoutArgs(const std::string& idetifier) {
            std::vector<Expression*> args{};
            IdentifierExpression id (idetifier);
            return newLink<class CallExpression>(id, args);
        }

        static Link<class CallExpression>  Call(const std::string& idetifier, const std::vector<Expression*>& args) {
            IdentifierExpression id (idetifier);
            return newLink<class CallExpression>(id, args);
        }

        static Link<class NumberExpression> Number(float value) {
            return newLink<class NumberExpression>(value);
        }

        static NumberExpression* NumberRaw(float value) {
            return new NumberExpression (value);
        }

        static StringExpression* StringRaw(const std::string& value) {
            return new StringExpression(value);
        }
    };
}
#endif //__LOLI_EXPRESSION_FACTORY_H__
