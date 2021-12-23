#include "daphnie.h"
#include "expression.h"
#include "loliLang/exceptions.h"
#include "token.h"
#include "utils.h"
#include <stdexcept>


bool loli::Daphnie::IsBinary (const loli::Token& value) {
    return IsMatchTo(value.forma(), _binaryOps);
}

bool loli::Daphnie::IsClosing(const loli::Token& value) {
    return IsMatchTo(value.forma(), {loli::Forma::RPAREN, loli::Forma::RCURL, loli::Forma::ELSE});
}

bool loli::Daphnie::IsMatchTo (loli::Forma value, const std::vector<loli::Forma>& to) {
    return std::find(to.begin(), to.end(), value) != to.end();
}

loli::Token loli::Daphnie::Peek() const {
    if (!IsEnd()) return _source[_current];
    return *(_source.end() - 1);
}

loli::Token loli::Daphnie::PeekNext() const {
    return _source[_current + 1];
}
loli::Token loli::Daphnie::PeekPrev() const {
    if (_current - 1 >= 0) return _source[_current - 1];
    return Peek();
}
        
bool loli::Daphnie::IsEnd() const {
    return _current >= _source.size() ||  _source[_current].forma() == Forma::EOF_;
}

loli::Daphnie& loli::Daphnie::MoveToNextBy(size_t steps) {
    _current += steps;
    return *this;
}

loli::Daphnie& loli::Daphnie::MoveToNext() {
    return MoveToNextBy(1);
}

loli::Expression* loli::Daphnie::AnyRules (std::stack<Expression*>& expressionsStack) {
    for (auto& rule: _rules) {
       auto temp = rule.ValidateOn(Peek().forma(), expressionsStack);
       if (temp.HasValue()) {
           return temp.Value();
       } else {
           continue;
       }
    }
    return nullptr;
}

loli::Expression* loli::Daphnie::growTree () {
    auto current = Peek();
    std::stack <Expression*> expressionsStack{};
    while (!IsEnd() && !IsClosing(current)) {
        auto res = AnyRules(expressionsStack);
        if (res != nullptr) {
            expressionsStack.push(res);
        }
        else if (IsMatchTo(current.forma(), {loli::Forma::SEMI})) {
            //TODO: what should i do with semi ?
        }
        else {
            std::string message = "unknown lexeme: \"";
            message.append(current.lexeme()).append("\"");
            throw std::runtime_error{message};
        }
        current = MoveToNext().Peek();
    }
    if (expressionsStack.empty()) {
        std::string message = "expressionStack is empty. something went wrong with reading expressions before";
        message.append(" '").append(current.lexeme()).append("'")
                .append(" or '").append(PeekPrev().lexeme()).append("'");
        throw std::runtime_error{message};
    }
    auto result = expressionsStack.top();
    expressionsStack.pop();
    return result;
}

loli::Expression* loli::Daphnie::BinaryExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    if (expressionsStack.empty()) {
        return UnaryExpression(expressionsStack);
    }
    auto identifier = expressionsStack.top();
    expressionsStack.pop();
    std::string op = current.lexeme();
    auto left = MoveToNext().growTree();
    if (IsClosing(Peek())) {
        _current--;
    }
    return new class BinaryExpression (op, left, identifier);
}

loli::Expression* loli::Daphnie::LambdaExpression (std::stack<Expression*> &expressionsStack) {
    if (expressionsStack.empty()) {
        throw std::runtime_error {"there is nothing to work with"};
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

loli::Expression* loli::Daphnie::UnaryExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    if (!IsMatchTo(MoveToNext().Peek().forma(), {loli::Forma::NUM})) {
        std::string c = Peek().asString();
        throw std::invalid_argument{"'"+c+"' is not a number"};
    }
    auto number = dynamic_cast<class NumberExpression*> (NumberExpression(expressionsStack));
    return new class UnaryExpression(current.lexeme(), number);
}

loli::Expression* loli::Daphnie::NumberExpression (std::stack<Expression*> &expressionsStack) {
    auto current = Peek();
    auto expr = new loli::NumberExpression(loli::unwrap<void, float>(current.literal()));
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
    if (IsMatchTo(current.forma(), {Forma::LPAREN, Forma::LCURL})) {
        MoveToNext();
    }
    if (IsMatchTo(Peek().forma(), {loli::Forma::RPAREN, loli::Forma::RCURL, loli::Forma::EOF_, loli::Forma::SEMI})) {
        throw std::runtime_error{"there is nothing to group"};
    }
    auto res = growTree();
    auto c = Peek();
    if (!IsMatchTo(c.forma(), {loli::Forma::RPAREN, loli::Forma::RCURL})) {
        throw std::runtime_error {"there is no ')' or '}'"};
    }
    return new loli::GroupingExpression(res);
}

loli::Expression* loli::Daphnie::BoolExpression(std::stack<Expression*>& expressionsStack) {
    auto current = Peek();
    return new loli::BoolExpression(loli::unwrap<void, bool>(current.literal()));
}

loli::Expression* loli::Daphnie::IfExpression (std::stack<Expression*>& expressionsStack) {
    auto current = MoveToNext().Peek();
    if (!IsMatchTo(current.forma(), {loli::Forma::LPAREN})) {
        throw std::runtime_error{"there is no '(' after 'if' statement"};
    }
    
    if (IsClosing(PeekNext())) {
        throw std::logic_error{"there is no 'condition', the next token is '" + PeekNext().lexeme()+"'"};
    }
    
    auto condition = GroupingExpression(expressionsStack);
    if (IsClosing(PeekNext()) || IsMatchTo(PeekNext().forma(), {loli::Forma::EOF_})){
        throw std::invalid_argument {"there is no 'then' branch after '" + Peek().lexeme() + "' or before '" + PeekNext().lexeme()+"'"};
    }
    
    auto thenBranch = MoveToNext().growTree();

    if (!IsClosing(Peek()) || IsEnd()) {
        throw  loli::ElseBranchException {"there is no 'else' branch"};
    }
    auto elseBranch = MoveToNext().growTree();
    
    return new loli::IfExpression(condition, thenBranch, elseBranch);
}

loli::Expression* loli::Daphnie::ClassExpression (std::stack<Expression*>& expressionsStack) {
    if (IsClosing(PeekNext()) ||!IsMatchTo(PeekNext().forma(), {loli::Forma::INDENTIFIER})) {
        throw std::logic_error{"there is no name for class"};
    }
    auto name = dynamic_cast<loli::IdentifierExpression*>(MoveToNext().IdentifierExpression(expressionsStack));

    if (!IsMatchTo(PeekNext().forma(), {loli::Forma::SEMI, loli::Forma::LCURL})) {
        throw std::invalid_argument{"there is no ';' or '{'"};
    }

    // TODO: implement class body parsing
    if (IsMatchTo(PeekNext().forma(), {loli::Forma::LCURL})) {
        throw std::runtime_error {"class body is not implemented"};
    }


    return new loli::ClassExpression(name->value());
}
