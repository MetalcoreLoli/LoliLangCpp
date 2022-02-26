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
    template<typename T>
    concept DerivedFromExpression = std::is_base_of_v<Expression, T>;

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

        static class CallExpression*  CallWithoutArgsRaw(const std::string& idetifier) {
            std::vector<Expression*> args{};
            IdentifierExpression id (idetifier);
            return new class CallExpression(id, args);
        }

        static class CallExpression*  CallRaw(const std::string& idetifier, const std::vector<Expression*>& args) {
            IdentifierExpression id (idetifier);
            return new class CallExpression(id, args);
        }
        static Link<class NumberExpression> Number(float value) {
            return newLink<class NumberExpression>(value);
        }

        template<typename T, typename U> 
            requires DerivedFromExpression <T> && DerivedFromExpression <U>
        static BinaryExpression BinaryValue (const std::string& op, T left, U right) {
            return BinaryExpression(op, &left, &right);
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

#define LOLI_NUM(num) loli::NumberExpression((num))
#define LOLI_NUMPTR(num) new LOLI_NUM(num)

#define LOLI_IDN(name) loli::IdentifierExpression((name))
#define LOLI_IDNPTR(name) new loli::IdentifierExpression((name))

#define LOLI_OP(op, left, right) loli::BinaryExpression((op), (left), (right))
#define LOLI_OPPTR(op, left, right) new LOLI_OP(op, left, right) 

#define LOLI_IF(condition, thn, els) loli::IfExpression((condition), (thn), (els))
#define LOLI_IFPTR(condition, thn, els) new LOLI_IF(condition, thn, els)

#define LOLI_FUNCPTR (name, args, body) loli::ExpressionFactory::LambdaRaw((name), (args), (body)) 
}
#endif //__LOLI_EXPRESSION_FACTORY_H__
