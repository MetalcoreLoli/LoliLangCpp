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
        
        class RuleOfExpressionInterpretaion;

        using ClassProperties = std::vector<Link<LambdaExpression>>;

        size_t _current{};
        std::vector<Token> _source {};

        std::vector<loli::Forma> _binaryOps{};
        std::vector<RuleOfExpressionInterpretaion> _rules{};

        [[nodiscard]] loli::Token Peek() const;
        [[nodiscard]] loli::Token PeekNext() const;
        [[nodiscard]] loli::Token PeekPrev() const;

        [[nodiscard]] bool IsBinary (const loli::Token& value);
        [[nodiscard]] bool IsClosing (const loli::Token& value);
        [[nodiscard]] bool IsMatchTo (loli::Forma value, const std::vector<loli::Forma>& to);

        Daphnie& MoveToNext();
        Daphnie& MoveToNextBy(size_t steps);
        
        [[nodiscard]] bool IsEnd() const;

        Expression* AnyRules (std::stack<Expression*>& expressionsStack);

        Expression* UnaryExpression  (std::stack<Expression*> &expressionsStack);
        Expression* BinaryExpression (std::stack<Expression*> &expressionsStack);
        Expression* NumberExpression (std::stack<Expression*> &expressionsStack);
        Expression* StringExpression (std::stack<Expression*> &expressionsStack);
        Expression* LambdaExpr (std::stack<Expression*> &expressionsStack);
        Expression* IdentifierExpression (std::stack<Expression*> &expressionsStack);
        Expression* GroupingExpression (std::stack<Expression*>& expressionsStack);
        Expression* IfExpression (std::stack<Expression*>& expressionsStack);
        Expression* BoolExpression (std::stack<Expression*>& expressionsStack);
        Expression* ClassExpression (std::stack<Expression*>& expressionsStack);


        ClassProperties ClassBodyExpression (std::stack<Expression*>& expressionsStack);

        class RuleOfExpressionInterpretaion {
            private:
                bool (loli::Daphnie::*_isMatchTo)(loli::Forma, const std::vector<loli::Forma>&);
                Expression* (loli::Daphnie::*_interpMethod)(std::stack<Expression*>&);

                std::vector<loli::Forma> _formas;

                Daphnie* _daphine;

            public:
                RuleOfExpressionInterpretaion(RuleOfExpressionInterpretaion&&) = default;
                RuleOfExpressionInterpretaion(RuleOfExpressionInterpretaion&) = default;

                explicit RuleOfExpressionInterpretaion (
                        std::vector<loli::Forma>  formas,
                        bool (loli::Daphnie::*isMatchTo)(loli::Forma, const  std::vector<loli::Forma>&),
                        Expression* (loli::Daphnie::*interpMethod)(std::stack<Expression*>&),
                        Daphnie* daphnie)
                : _formas(std::move(formas)), _isMatchTo (isMatchTo), _interpMethod(interpMethod), _daphine(daphnie){}
                
                Maybe<Expression*> ValidateOn(loli::Forma forma, std::stack<Expression*>& expressionsStack) {
                    if ((_daphine->*_isMatchTo)(forma, _formas)) {
                        return Maybe<Expression*>((_daphine->*_interpMethod)(expressionsStack));
                    }
                    return Maybe<Expression*>(nullptr);
                }
        };
    
    public:
        Expression* growTree();


        explicit Daphnie(std::vector<Token>  source) : _source(std::move(source)) {
            _binaryOps.push_back(loli::Forma::ADD);
            _binaryOps.push_back(loli::Forma::SUB);
            _binaryOps.push_back(loli::Forma::MUL);
            _binaryOps.push_back(loli::Forma::DIV);
            _binaryOps.push_back(loli::Forma::EQ);

            _rules.emplace_back(_binaryOps, &Daphnie::IsMatchTo, &Daphnie::BinaryExpression, this);
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::NUM}, &Daphnie::IsMatchTo, &Daphnie::NumberExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::STRING_LIT}, &Daphnie::IsMatchTo, &Daphnie::StringExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::CLASS}, &Daphnie::IsMatchTo, &Daphnie::ClassExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::LAMBDA_ARROW, loli::Forma::DEFINE}, &Daphnie::IsMatchTo, &Daphnie::LambdaExpr, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::INDENTIFIER}, &Daphnie::IsMatchTo, &Daphnie::IdentifierExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::IF}, &Daphnie::IsMatchTo, &Daphnie::IfExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::LPAREN, loli::Forma::LCURL}, &Daphnie::IsMatchTo, &Daphnie::GroupingExpression, this));
            _rules.push_back(RuleOfExpressionInterpretaion({loli::Forma::TRUE, loli::Forma::FALSE}, &Daphnie::IsMatchTo, &Daphnie::BoolExpression, this));
        }
    };
};
#endif
