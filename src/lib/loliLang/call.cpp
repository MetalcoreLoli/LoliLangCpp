#include  "call.h"
#include <string>

loli::Call& loli::Call::Validate (const loli::CallExpression& value, loli::MemoryTableOfExpressions& memory) {
    Expression* out = nullptr;
    auto nameSpec = ExpressionSpecFactory::LambdaExpressionNameSpec(value.idetifier().value()).get();
    if (!memory.TryFind(nameSpec, &out)) {
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
    _local.PushIntoMainStack(&_lambda);
    for (size_t i = 0; i < _lambda.args().size(); i++) {
        auto name = _lambda.args()[i].value();
        auto arg = _callArgs[_lambda.args().size() - 1 - i];
        _mappedArgs.push_back(ExpressionFactory::LambdaRaw(name, arg));
    }
    return *this;
}

loli::Call& loli::Call::FillLocalStackFrame(loli::Lexy& global) {
    for (auto a : _mappedArgs) {
        if (a->IsLiteral)
            _local.PushIntoMainStack(a);
        else {
            auto result =  (a->body()->visit(&global));
            _local.PushIntoMainStack(ExpressionFactory::LambdaRaw(a->identifier().value(), ExpressionFactory::FromReturnResult(result)));
        }
    }
    return *this;
}
loli::ReturnResult loli::Call::Execute() {
    return _lambda.body()->visit(&_local);
}