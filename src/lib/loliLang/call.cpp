#include  "call.h"
#include "loliLang/memory.h"
#include <string>

loli::Call& loli::Call::Validate (const loli::CallExpression& value, mem::IEnvironment* local) {
    Expression* out = nullptr;
    auto nameSpec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.idetifier().value());
    if (!_globalEnv->TryFind(nameSpec, &out) && !local->TryFind(nameSpec, &out)) {
        utils::ThrowHelper::Throw_ThereIsNo(value.idetifier().value()); 
    }
    _lambda = *(dynamic_cast <LambdaExpression*>(out));
    if (_lambda.args().size() > value.args().size()) {
        throw std::runtime_error {
            "There is a missing arg in call of `"+value.idetifier().value()+"` function"};
    } else if (_lambda.args().size() < value.args().size()) {
        throw std::runtime_error {
            "There is a extra arg in call of `"+value.idetifier().value()+"` function"};
    }

    _callArgs = std::vector (value.args().begin(), value.args().end());
    return *this;
}


loli::Call& loli::Call::Map () {
    for (size_t i = 0; i < _lambda.args().size(); i++) {
        auto name = _lambda.args()[i].value();
        auto arg = _callArgs[_lambda.args().size() - 1 - i];
        _mappedArgs.push_back(ExpressionFactory::LambdaRaw(name, arg));
    }
    return *this;
}

loli::Call& loli::Call::FillLocalStackFrame(loli::Lexy& global) {
    _local.PushIntoMainStack(&_lambda);
    for (auto a : _mappedArgs) {
        auto result =  (a->body()->visit(&global));
        _local.PushIntoMainStack(ExpressionFactory::LambdaRaw(a->identifier().value(), ExpressionFactory::FromReturnResult(result)));
    }
    if (_lambda.hasWhereBlock()) {
        for (auto arg: _lambda.where()) {
            _local.PushIntoMainStack(arg);
        }
    }
    return *this;
}
loli::ReturnResult loli::Call::Execute() {
    return _lambda.body()->visit(&_local);
}
