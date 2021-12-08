#ifndef __DAPHNIE_H__
#define __DAPHNIE_H__

#include <string>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>

#include "./token.h"
#include "./utils.h"
#include "./expression.h"

namespace loli { 
    class Daphnie {

        size_t _current{};
        std::vector<Token> _source {};
        
        loli::Token Peek() const;
        loli::Token PeekNext() const;

        bool IsBinary (loli::Token value) const;
        bool IsMatchTo (loli::Forma value, loli::Forma to);
        
        Daphnie& MoveToNext();
        
        bool IsEnd() const;

        Expression* BinaryExpression (std::stack<Expression*> &expressionsStack);
        Expression* NumberExpression (std::stack<Expression*> &expressionsStack);
        Expression* StringExpression (std::stack<Expression*> &expressionsStack);
        Expression* LambdaExpression (std::stack<Expression*> &expressionsStack);
        Expression* IndentifierExpression (std::stack<Expression*> &expressionsStack);
    
    public:
        Expression* growTree();

        explicit Daphnie(const std::vector<Token>& source) : _source(source) {
        }
    };
};
#endif
