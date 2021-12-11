#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <any>
#include <string>
#include <memory>
#include <vector>


#include "utils.h"

namespace loli {
    using namespace utils;
    class Expression;
    class IdentifierExpression;
    class NumberExpression;
    class BinaryExpression;
    class LambdaExpression;
    class StringExpression;
    class IfExpression;
    class GroupingExpression;
    class BoolExpression;
    
    struct IVisitor {
        virtual GenericLink visitBinaryExpression (BinaryExpression& value) = 0;
        virtual GenericLink visitNumberExpression (NumberExpression& value) = 0;
        virtual GenericLink visitLambdaExpression (LambdaExpression& value) = 0;
        virtual GenericLink visitIdentifierExpression (IdentifierExpression& value) = 0;
        virtual GenericLink visitStringExpression (StringExpression& value) = 0;
        virtual GenericLink visitIfExpression (IfExpression& value) = 0;
        virtual GenericLink visitGroupingExpression (GroupingExpression& value) = 0;
        virtual GenericLink visitBoolExpression (BoolExpression& value) = 0;
    };

    struct Expression {
        virtual GenericLink visit (IVisitor * visitor) = 0;
    };

    class StringExpression : public Expression {
        private:
            std::string _value;
        public:
            std::string& value() const { return const_cast<std::string&>(_value); }

            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitStringExpression(*this);
            }
            explicit StringExpression (const std::string& value) ;
    };

    class NumberExpression : public Expression {
        private:
            float _value;
        public:
            float value() const ;
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitNumberExpression(*this);
            }
            explicit NumberExpression (float value) ;
    };

    class BinaryExpression : public Expression {
        private:
            std::string _operand;
            Expression* _left;
            Expression* _right;

        public:
            Expression* left  () const;
            Expression* right () const;

            std::string& operand() const { return const_cast<std::string&>(_operand); }


            BinaryExpression& left  (Expression* value);
            BinaryExpression& right (Expression* value);

            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitBinaryExpression(*this);
            }

            BinaryExpression (const std::string& operand, Expression* left, Expression* right);
    };

    class IdentifierExpression : public Expression {
        private:
            std::string _value;

        public:
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitIdentifierExpression(*this);
            }

            std::string value() const { return _value; }
            explicit IdentifierExpression (const std::string& value);
            IdentifierExpression () {}

    };

    class LambdaExpression : public Expression {
        private:
            IdentifierExpression _idetifier;
            Expression*          _body;
            std::vector<IdentifierExpression> _args{};

        public:
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitLambdaExpression(*this);
            }

            std::vector<IdentifierExpression>& args() const { return const_cast<std::vector<IdentifierExpression>&>(_args); }
            IdentifierExpression& identifier() const { return const_cast<IdentifierExpression&>(_idetifier); }
            Expression*           body () const { return _body; }

            LambdaExpression(IdentifierExpression& identifier, Expression* body);
            LambdaExpression(IdentifierExpression& identifier, std::vector<IdentifierExpression>& args, Expression* body);
    };

    class IfExpression : public Expression {
        private:
            Expression* _condition;
            Expression* _then; 
            Expression* _else;
        public:
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitIfExpression(*this);
            }

            Expression* condition() const { return _condition; }
            Expression* then () const { return _then; }
            Expression* els () const { return _else; }

            IfExpression(Expression* condition, Expression* then, Expression* els) 
                : _condition(condition), _then(then), _else(els) {}
    };

    class GroupingExpression : public Expression {
        private:
            Expression* _expression;
        public:
            Expression* expression () const { return _expression; }
        
            loli::GenericLink visit (IVisitor * visitor) {
                return visitor->visitGroupingExpression(*this);
            }

            explicit GroupingExpression (Expression* expr) : _expression(expr){}
    };

    class BoolExpression : public Expression {
        private:
            bool _value = false;
        public:


            loli::GenericLink visit (IVisitor * visitor) {
                return visitor->visitBoolExpression(*this);
            }

            bool value () const { return _value; }
            explicit BoolExpression (bool value) : _value(value) {}
    };
};
#endif// __EXPRESSION_H__ 
