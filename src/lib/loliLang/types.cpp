#include "loliLang/types.h"
#include "loliLang/call.h"
#include "loliLang/expression.h"
#include "loliLang/expressionFactory.hpp"
#include "loliLang/memory.h"
#include "loliLang/utils.h"
#include "loliLang/ast.cpp"
#include <string>

loli::ReturnResult loli::TypeChecker::visitBinaryExpression (BinaryExpression& value) {
//    utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitBinaryExpression");
//    return ReturnResult::Empty();
    auto left   = value.left()->visit(this);
    auto right  = value.right()->visit(this);
    if (left.TypeHashCode() != right.TypeHashCode()) {
        throw std::runtime_error {"type of left value is not the same as type of right value"};
    }
    std::vector <std::string> _ops = {"==", "!=", ">", "<", "<=", ">="};
    if (std::find(_ops.begin(), _ops.end(), value.operand()) != _ops.end()) {
        return ReturnResult::New(BoolType());
    }
    return left;
} 

loli::ReturnResult loli::TypeChecker::visitNumberExpression (NumberExpression& value) {
    return {utils::newLink<FloatType>(), typeid(FloatType).hash_code()};
} 
loli::ReturnResult loli::TypeChecker::visitLambdaExpression (LambdaExpression& value) {
    return value.body()->visit(this);
} 
loli::ReturnResult loli::TypeChecker::visitIdentifierExpression (IdentifierExpression& value) {
    //utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitIdentifierExpression");
    //return ReturnResult::Empty();

    Expression* out = nullptr;
    auto spec = loli::ExpressionSpecFactory::LambdaExpressionNameSpec(value.value());
    auto mem = mem::Or(_global, _local);
    if (!mem.TryFind(spec, &out)) {
        utils::ThrowHelper::Throw_ThereIsNo(value.value());  
    }
    return out->visit(this);
} 
loli::ReturnResult loli::TypeChecker::visitStringExpression (StringExpression& value) {
    return {utils::newLink<StringType>(), typeid(StringType).hash_code()};
} 
loli::ReturnResult loli::TypeChecker::visitIfExpression (IfExpression& value) {
    auto condition = value.condition()->visit(this);
    if (condition.TypeHashCode() != typeid(BoolType).hash_code()) {
        throw std::runtime_error {"There should be boolen expression inside of a condition block"};
    }

    auto thn = value.then()->visit(this);
    auto els = value.els()->visit(this);
    
    if (thn.TypeHashCode() != els.TypeHashCode()) {
        throw std::runtime_error {"type of left value is not the same as type of right value"};
    }
    return thn;
} 
loli::ReturnResult loli::TypeChecker::visitGroupingExpression (GroupingExpression& value) {
    return value.expression()->visit(this);
} 
loli::ReturnResult loli::TypeChecker::visitBoolExpression (BoolExpression& value) {
    return {utils::newLink<BoolType>(), typeid(BoolType).hash_code()};
} 
loli::ReturnResult loli::TypeChecker::visitUnaryExpression (UnaryExpression& value) {
    utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitUnaryExpression");
    return ReturnResult::Empty();
} 
loli::ReturnResult loli::TypeChecker::visitClassExpression (ClassExpression& value) {
    utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitClassExpression");
    return ReturnResult::Empty();
} 
loli::ReturnResult loli::TypeChecker::visitBodyExpression (BodyExpression& value) {
    utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitBodyExpression");
    return ReturnResult::Empty();
} 
loli::ReturnResult loli::TypeChecker::visitForExpression (ForExpression& value) {
    utils::ThrowHelper::Throw_NotImplemented("loli::TypeChecker::visitForExpression");
    return ReturnResult::Empty();
} 
loli::ReturnResult loli::TypeChecker::visitCallExpression (CallExpression& value) {
    Expression* out = nullptr;
    auto spec = loli::ExpressionSpecFactory::LambdaExpressionNameSpec(value.idetifier().value());
    auto mem = mem::Or(_global, _local);
    if (!mem.TryFind(spec, &out)) {
        utils::ThrowHelper::Throw_ThereIsNo(value.idetifier().value());  
    }
    if (value.args().empty()) {
        return out->visit(this);
    }
    auto func = dynamic_cast<LambdaExpression*>(out);

    //mapping
    for (auto f: func->where()) {
        _local->Push(f);    
    }

    for (size_t i = 0; i < value.args().size(); i++) {
        auto arg = value.args()[i];
        _local->Push(LOLI_FUNCAPTR(func->args()[i].value(), arg));
    }
     
    return func->visit(this);
} 

loli::IMethod* loli::TypeMethodGetRequest::GetMethodOfBoolType (loli::ITypeRequestHander* type, std::string_view methodName) {
    if (!type->ContainsMethod(methodName)) {
        utils::ThrowHelper::Throw_OperationIsNotImplementedForType(std::string(methodName), "Bool");
    }
    return type->GetMethod(methodName);
} 

loli::IMethod* loli::TypeMethodGetRequest::GetMethodOfFloatType(loli::ITypeRequestHander* type, std::string_view methodName){
    if (!type->ContainsMethod(methodName)) {
        utils::ThrowHelper::Throw_OperationIsNotImplementedForType(std::string(methodName), "Number");
    }
    return type->GetMethod(methodName);
} 

loli::IMethod* loli::TypeMethodGetRequest::GetMethodOfStringType (const StringType& type, std::string_view methodNameHashCode) {
    utils::ThrowHelper::Throw_NotImplemented("loli::TypeMethodGetRequest::GetMethodOfStringType "); 
    return nullptr;
} 

bool loli::FloatTypeRequestHandler::ContainsMethod (std::string_view methodName) {
    return _table.contains(methodName);
} 

loli::IMethod* loli::FloatTypeRequestHandler::GetMethod (std::string_view methodName) {
    return _table.at(methodName); 
} 

bool loli::BoolTypesRequestHandler::ContainsMethod (std::string_view methodName) {
    return _table.contains(methodName);
} 

loli::IMethod* loli::BoolTypesRequestHandler::GetMethod (std::string_view methodName) {
    return _table.at(methodName); 
} 
loli::ReturnResult loli::TypeChecker::visitWhereExpression (WhereExpression& value) {
    ThrowHelper::Throw_NotImplemented("loli::Lexy::visitWhereExpression");
    return loli::ReturnResult::Empty();
}
