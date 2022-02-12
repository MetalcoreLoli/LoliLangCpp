#ifndef __LOLI_TYPES__
#define __LOLI_TYPES__
#include <cstddef>
#include <functional>
#include <string>
#include <map>
#include <string_view>

#include "loliLang/expression.h"
#include "loliLang/memory.h"
#include "loliLang/utils.h"

namespace loli {
    class BoolType;
    class FloatType;
    class StringType;

    struct IMethod { 
        virtual ReturnResult Invoke(const std::vector<ReturnResult>& args) = 0;
    };
    
    struct ITypeMethodGetter  {
        virtual IMethod* GetMethodOfBoolType (const BoolType& type, std::string_view methodNameHashCode) = 0;
        virtual IMethod* GetMethodOfFloatType (const FloatType& type, std::string_view methodNameHashCode) = 0;
        virtual IMethod* GetMethodOfStringType (const StringType& type, std::string_view methodNameHashCode) = 0;
    };

    struct TypeMethodGetter : public ITypeMethodGetter {
        IMethod* GetMethodOfBoolType (const BoolType& type, std::string_view methodNameHashCode) override;
        IMethod* GetMethodOfFloatType (const FloatType& type, std::string_view methodNameHashCode) override;
        IMethod* GetMethodOfStringType (const StringType& type, std::string_view methodNameHashCode) override;

        private: 
    }; 

    template<typename T>
    struct AddMethodImpl : public IMethod {
        ReturnResult Invoke (const std::vector<ReturnResult>& args) override {
            T sum;
            for (auto arg: args) {
                sum += arg.Unwrap<T>();
            }
            return ReturnResult::New(sum);
        }
    };

    struct IType {
        virtual IMethod* GetMethod(ITypeMethodGetter* getter, std::string_view methodName) = 0;
    };


    class FloatType : public IType {

        public:
            //TODO: FIX THIS !!!!!!!!1
            std::map<std::string_view, IMethod*> VTable{};
            IMethod* GetMethod(ITypeMethodGetter* getter, std::string_view methodName) override {
                return getter->GetMethodOfFloatType(*this, methodName);
            } 

            FloatType () {
                VTable.insert({"+", new AddMethodImpl<float>});
            }
    };

    class BoolType : public IType {
        public: 
            IMethod* GetMethod(ITypeMethodGetter* getter, std::string_view methodNameHashCode) override {
                return getter->GetMethodOfBoolType(*this, methodNameHashCode);
            } 
    };
    class StringType : public IType {
        public: 
            IMethod* GetMethod(ITypeMethodGetter* getter, std::string_view methodNameHashCode) override {
                return getter->GetMethodOfStringType(*this, methodNameHashCode);
            } 
    };


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
