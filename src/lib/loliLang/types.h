#ifndef __LOLI_TYPES__
#define __LOLI_TYPES__
#include <functional>
#include <string>
#include <map>
#include <string_view>

#include "loliLang/expression.h"
#include "loliLang/memory.h"
#include "loliLang/utils.h"

namespace loli {
    template <typename T>
    class TypeVTable {
        using BinaryOpFunction = std::function <ReturnResult(T, T)>;
        using BinaryOpsTable = std::map <std::string_view, BinaryOpFunction>;
        BinaryOpsTable _binaryOps {};

        public:
            TypeVTable(){}
            BinaryOpsTable BinaryOps () const {
                return const_cast<BinaryOpsTable&>(_binaryOps);
            }
            
            BinaryOpFunction BinaryOp (std::string_view sv)  {
                if (_binaryOps.contains(sv)) return _binaryOps[sv];
                else {
                    utils::ThrowHelper::Throw_OperationIsNotImplementedForType (std::string(sv), typeid(T).name());
                }
            }
    };

    template <typename T>
    class Type {
        protected:
            TypeVTable<T> _vTable{};

        public:
            Type (){}
            virtual TypeVTable<T>& VTable() const {return const_cast<TypeVTable<T>&>(_vTable);}

    };


    class FloatType : public Type<float> {
        public:
            FloatType () {
                _vTable.BinaryOps().insert({"+", [](float a, float b) -> ReturnResult {return {newLink<float>(a+b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({"-", [](float a, float b) -> ReturnResult {return {newLink<float>(a-b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({"*", [](float a, float b) -> ReturnResult {return {newLink<float>(a*b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({"/", [](float a, float b) -> ReturnResult {return {newLink<float>(a/b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({"<", [](float a, float b) -> ReturnResult {return {newLink<float>(a<b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({">", [](float a, float b) -> ReturnResult {return {newLink<float>(a>b), typeid(float).hash_code()};}});
                _vTable.BinaryOps().insert({"!=", [](float a, float b) -> ReturnResult {return {newLink<bool>(a!=b), typeid(bool).hash_code()};}});
                _vTable.BinaryOps().insert({"==", [](float a, float b) -> ReturnResult {return {newLink<bool>(a==b), typeid(bool).hash_code()};}});
            }
    };

    class BoolType : public Type<bool> {};
    class StringType : public Type<std::string> {};


    class TypeChecker : public IVisitor {
        private: 
            mem::IEnvironment* _env;
        public:
            TypeChecker () : _env (&mem::GlobalEnvironment) {}
            TypeChecker (mem::IEnvironment* env) : _env (env) {}
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
    };
}
#endif // __LOLI_TYPES__
