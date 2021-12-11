#include "daphnie.h"
#include "expression.h"
#include "token.h"
#include "utils.h"
#include <stdexcept>


bool loli::Daphnie::IsBinary (const loli::Token& value) const {
    return IsMatchTo(value.forma(), _binaryOps);
}

bool loli::Daphnie::IsMatchTo (loli::Forma value, const std::vector<loli::Forma>& to) const {
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

loli::Daphnie& loli::Daphnie::MoveToNextBy(size_t steps) {
    _current += steps;
    return *this;
}

loli::Daphnie& loli::Daphnie::MoveToNext() {
    return MoveToNextBy(1);
}

loli::Expression* loli::Daphnie::growTree () {
    auto current = Peek();
    std::stack <Expression*> expressionsStack{};
    while (!IsEnd()) {
        if (IsMatchTo(current.forma(), {loli::Forma::INDENTIFIER})) {
            expressionsStack.push(new loli::IdentifierExpression(current.lexeme()));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::STRING_LIT})) {
            expressionsStack.push (StringExpression(expressionsStack));
        }
        else if (IsMatchTo (current.forma(), {loli::Forma::LAMBDA_ARROW})) {
            expressionsStack.push (LambdaExpression(expressionsStack));
        } 
        else if (IsBinary(current)) {
            expressionsStack.push(BinaryExpression(expressionsStack));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::NUM})) {
            expressionsStack.push(NumberExpression(expressionsStack));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::SEMI})) {
            //TODO: what should i do with semi ?
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::IF})) {
            expressionsStack.push(IfExpression(expressionsStack));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::LPAREN})) {
            expressionsStack.push(GroupingExpression(expressionsStack));
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::RPAREN})) {
            _current--;
            break;
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::ELSE})) {
            _current--;
            break;
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::TRUE, loli::Forma::FALSE})) {
            expressionsStack.push(BoolExpression(expressionsStack));
        }
        else {
            std::string message = "unknown lexeme: \"";
            message.append(current.lexeme()).append("\"");
            throw std::runtime_error{message};
        }
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
    std::vector<class IdentifierExpression> identifiers{};
    
    while (!expressionsStack.empty()) {
        auto identifier = dynamic_cast<class IdentifierExpression*>(expressionsStack.top());
        expressionsStack.pop();
        identifiers.push_back(*identifier);
    }

    auto name = *(identifiers.end() - 1);
    auto body = MoveToNext().growTree();

    if (identifiers.size() > 1) {
        auto args = std::vector<class IdentifierExpression>(identifiers.begin(), identifiers.end() - 1);
        return new loli::LambdaExpression(name, args, body);
    }
    return new loli::LambdaExpression(name, body);
}

loli::Expression* loli::Daphnie::NumberExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::NumberExpression(loli::unwrap<void, float>(current.literal()));
    expressionsStack.push(expr);
    return expr;
}

loli::Expression* loli::Daphnie::StringExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::StringExpression(current.lexeme());
    return expr;
}

loli::Expression* loli::Daphnie::IdentifierExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr =new class IdentifierExpression(current.lexeme());
    return expr;
}

loli::Expression* loli::Daphnie::GroupingExpression (std::stack<Expression*>& expressionsStack) {
    auto current = Peek();
    auto res = MoveToNext().growTree();
    return new loli::GroupingExpression(res);
}

loli::Expression* loli::Daphnie::BoolExpression(std::stack<Expression*>& expressionsStack) {
    auto current = Peek();
    return new loli::BoolExpression(loli::unwrap<void, bool>(current.literal()));
}

loli::Expression* loli::Daphnie::IfExpression (std::stack<Expression*>& expressionsStack) {
    auto next = PeekNext();
    if (!IsMatchTo(next.forma(), {loli::Forma::LPAREN})) {
        throw std::runtime_error{"there is no '(' after 'if' statement"};
    }

    auto condition = MoveToNext().GroupingExpression(expressionsStack);
    if (IsMatchTo(MoveToNextBy(2).Peek().forma(), {loli::Forma::RPAREN, loli::Forma::EOF_, loli::Forma::SEMI})) {
        throw std::runtime_error{"there is no 'then' branch"};
    }
    auto then      = growTree(); 
    //auto then    = new loli::IdentifierExpression("'THEN BRANCH IS NOT IMPLEMENTED'"); 

    if (!IsMatchTo(PeekNext().forma(), {loli::Forma::ELSE}) || 
         IsMatchTo(MoveToNextBy(2).Peek().forma(), {loli::Forma::EOF_, loli::Forma::SEMI})) {
        throw std::runtime_error{"there is no else branch"};
    }
    auto els       = growTree();
    
    return new loli::IfExpression(condition, then, els);
}
