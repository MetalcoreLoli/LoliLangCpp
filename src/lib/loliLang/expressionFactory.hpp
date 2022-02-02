#ifndef __LOLI_EXPRESSION_FACTORY_H__
#define __LOLI_EXPRESSION_FACTORY_H__

#include "expression.h"
#include "utils.h"
#include <any>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>

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

        static Expression* FromReturnResult (ReturnResult result) {
            static std::map<size_t, std::function<Expression*(ReturnResult)>> mm = {
                {typeid(float).hash_code(), [](ReturnResult a){return NumberRaw(a.Unwrap<float>());}},
                {typeid(std::string).hash_code(), [](ReturnResult a){return StringRaw(a.Unwrap<std::string>());}},
            };
            size_t hash_code =  result.TypeHashCode();
            if (mm.contains(hash_code)) {
                return mm[hash_code](result);
            } else {
                throw std::runtime_error{"I don't know how to create expression from type with hash code: `"+ std::string(typeid(result.TypeHashCode()).name())+"`"};
            }
        }
    };
}
#endif //__LOLI_EXPRESSION_FACTORY_H__
