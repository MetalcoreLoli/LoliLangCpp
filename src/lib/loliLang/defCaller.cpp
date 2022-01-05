#include "defCaller.h"

loli::GenericLink loli::DefCaller::callLambdaExpression(LambdaExpression& value, std::stack<Expression*>& stackFrame) {
    loli::Lexy lexy{};
    if (value.args().empty()) {
        for (auto& arg : value.args()) {

            auto param = new loli::LambdaExpression(arg, stackFrame.top());
            stackFrame.pop();
        }
    } else {
        return value.body()->visit(&lexy);
    }
    return nullptr;
}
