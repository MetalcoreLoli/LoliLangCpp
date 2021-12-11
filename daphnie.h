#ifndef __DAPHNIE_H__
#define __DAPHNIE_H__

#include <string>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <assert.h>

#include "./token.h"
#include "./utils.h"
#include "./expression.h"

namespace loli { 
    class Daphnie {

        size_t _current{};
        std::vector<Token> _source {};

        std::vector<loli::Forma> _binaryOps{};         
        loli::Token Peek() const;
        loli::Token PeekNext() const;

        bool IsBinary (loli::Token value) const;
        bool IsMatchTo (loli::Forma value, const std::vector<loli::Forma>& to) const;
        
        Daphnie& MoveToNext();
        Daphnie& MoveToNextBy(size_t steps);
        
        bool IsEnd() const;

        Expression* BinaryExpression (std::stack<Expression*> &expressionsStack);
        Expression* NumberExpression (std::stack<Expression*> &expressionsStack);
        Expression* StringExpression (std::stack<Expression*> &expressionsStack);
        Expression* LambdaExpression (std::stack<Expression*> &expressionsStack);
        Expression* IndentifierExpression (std::stack<Expression*> &expressionsStack);
        Expression* GroupingExpression (std::stack<Expression*>& expressionsStack);
        Expression* IfExpression (std::stack<Expression*>& expressionsStack);
        Expression* BoolExpression (std::stack<Expression*>& expressionsStack);
    
    public:
        Expression* growTree();

        explicit Daphnie(const std::vector<Token>& source) : _source(source) {
            _binaryOps.push_back(loli::Forma::ADD);
            _binaryOps.push_back(loli::Forma::SUB);
            _binaryOps.push_back(loli::Forma::MUL);
            _binaryOps.push_back(loli::Forma::DIV);
            _binaryOps.push_back(loli::Forma::EQ);
        }
    };
};
#endif
