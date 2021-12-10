#include "daphnie.h"
#include "expression.h"
#include "token.h"
#include "utils.h"
#include <stdexcept>


bool loli::Daphnie::IsBinary (loli::Token value) const {
    return Peek().forma() == value.forma();
}

bool loli::Daphnie::IsMatchTo (loli::Forma value, const std::vector<loli::Forma>& to) {
    return std::find(to.begin(), to.end(), value) != to.end();
}

loli::Token loli::Daphnie::Peek() const {
    if (!IsEnd()) return _source[_current];
    return *(_source.end() - 1);
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
        if (IsMatchTo(current.forma(), {loli::Forma::INDENTIFIER})) {
            expressionsStack.push(new loli::IdentifierExpression(current.lexeme()));
        }
        else if (current.forma() == loli::Forma::STRING_LIT) {
            expressionsStack.push (StringExpression(expressionsStack));
        }
        else if (IsMatchTo (current.forma(), {loli::Forma::LAMBDA_ARROW})) {
            expressionsStack.push (LambdaExpression(expressionsStack));
        } 
        else if (IsMatchTo(current.forma(), _binaryOps)) {
            expressionsStack.push(BinaryExpression(expressionsStack));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::NUM})) {
            expressionsStack.push(NumberExpression(expressionsStack));
        }
        //else {
        //    std::string message = "unknown lexeme: \"";
        //    message.append(current.lexeme()).append("\"");
        //    throw std::runtime_error{message};
        //}
        current = MoveToNext().Peek();
    }
    auto result = expressionsStack.top();
    expressionsStack.pop();
    return result;
}

loli::Expression* loli::Daphnie::BinaryExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    if (expressionsStack.empty()) {
        throw std::runtime_error{"there is nothing to work with"};
    }
    auto identifier = expressionsStack.top();
    expressionsStack.pop();
    std::string op = current.lexeme();
    return new class BinaryExpression (op, MoveToNext().growTree(), identifier);
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

    if (identifiers.size() > 1) {
        auto args = std::vector<IdentifierExpression>(identifiers.begin(), identifiers.end() - 1);
        return new loli::LambdaExpression(name, args, body);
    }
    return new loli::LambdaExpression(name, body);
}

loli::Expression* loli::Daphnie::NumberExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::NumberExpression(loli::unwarp<void, float>(current.literal()));
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
