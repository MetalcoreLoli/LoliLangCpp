#include "daphnie.h"
#include "expression.h"
#include "token.h"
#include <stdexcept>


bool loli::Daphnie::IsBinary (loli::Token value) const {
    return Peek().forma() == value.forma();
}

bool loli::Daphnie::IsMatchTo (loli::Forma value, loli::Forma to) {
    return ((int)to & (int)value) == (int)value;
}

loli::Token loli::Daphnie::Peek() const {
    return _source[_current];
}

loli::Token loli::Daphnie::PeekNext() const {
    if (!IsEnd()) return _source[_current + 1];
    return Peek();
}
        
bool loli::Daphnie::IsEnd() const {
    return _current >= _source.size() || _source[_current].forma() == Forma::EOF_;
}

loli::Daphnie& loli::Daphnie::MoveToNext() {
    _current++;
    return *this;
}

loli::Expression* loli::Daphnie::growTree () {
    auto current = Peek();
    std::stack <Expression*> expressionsStack{};
    while (!IsEnd()) {
        if (IsMatchTo(current.forma(), loli::Forma::INDENTIFIER)) {
            expressionsStack.push(new loli::IdentifierExpression(current.lexeme()));
        }
        else if (current.forma() == loli::Forma::STRING_LIT) {
            expressionsStack.push (StringExpression(expressionsStack));
        }
        else if (IsMatchTo (current.forma(), loli::Forma::LAMBDA_ARROW)) {
            expressionsStack.push (LambdaExpression(expressionsStack));
        } 
        current = MoveToNext().Peek();
    }
    auto result = expressionsStack.top();
    expressionsStack.pop();
    return result;
}

loli::Expression* loli::Daphnie::BinaryExpression (std::stack<Expression*> &expressionsStack) {
    return nullptr;
}

loli::Expression* loli::Daphnie::LambdaExpression (std::stack<Expression*> &expressionsStack) {
    if (expressionsStack.empty()) {
        throw std::runtime_error{"there is no name und body for lambda"};
    }
    std::vector<IdentifierExpression> identifiers{};
    
    while (!expressionsStack.empty()) {
        auto identifier = static_cast<IdentifierExpression*>(expressionsStack.top());
        expressionsStack.pop();
        identifiers.push_back(*identifier);
    }

    auto name = *(identifiers.end() - 1);
    auto body = MoveToNext().growTree();
    auto expr = new loli::LambdaExpression(name, body);
    return expr;
}

loli::Expression* loli::Daphnie::NumberExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::NumberExpression(*std::static_pointer_cast<float>(current.literal()));
    expressionsStack.push(expr);
    return expr;
}

loli::Expression* loli::Daphnie::StringExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::StringExpression(current.lexeme());
    return expr;
}

loli::Expression* loli::Daphnie::IndentifierExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr =new IdentifierExpression(current.lexeme()); 
    return expr;
}
