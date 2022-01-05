#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <any>
#include <string>
#include <memory>
#include <vector>
#include <stack>


#include "utils.h"

namespace loli {
    using namespace utils;
    class Expression;
    class CallableExpression;

    class IdentifierExpression;
    class NumberExpression;
    class BinaryExpression;
    class LambdaExpression;
    class StringExpression;
    class IfExpression;
    class GroupingExpression;
    class BoolExpression;
    class UnaryExpression;
    class ClassExpression;
    class BodyExpression;
    class ForExpression;
    
    using VectorOfExprLinks = std::vector<loli::Link<Expression>>;

    struct IVisitor {
        virtual GenericLink visitBinaryExpression (BinaryExpression& value) = 0;
        virtual GenericLink visitNumberExpression (NumberExpression& value) = 0;
        virtual GenericLink visitLambdaExpression (LambdaExpression& value) = 0;
        virtual GenericLink visitIdentifierExpression (IdentifierExpression& value) = 0;
        virtual GenericLink visitStringExpression (StringExpression& value) = 0;
        virtual GenericLink visitIfExpression (IfExpression& value) = 0;
        virtual GenericLink visitGroupingExpression (GroupingExpression& value) = 0;
        virtual GenericLink visitBoolExpression (BoolExpression& value) = 0;
        virtual GenericLink visitUnaryExpression (UnaryExpression& value) = 0;
        virtual GenericLink visitClassExpression (ClassExpression& value) = 0;
        virtual GenericLink visitBodyExpression (BodyExpression& value) = 0;
        virtual GenericLink visitForExpression (ForExpression& value) = 0;
    };
    
    struct ICaller {
        virtual GenericLink callLambdaExpression(LambdaExpression& value, std::stack<Expression*>& stackFrame) = 0;
    };

    struct Expression {
        virtual GenericLink visit (IVisitor * visitor) = 0;
    };

    struct CallableExpression {
        virtual GenericLink call (
                ICaller* caller,
                std::stack<Expression*>& stackFrame) = 0;
    };

    class StringExpression : public Expression {
        private:
            std::string _value;
        public:
            [[nodiscard]] std::string& value() const { return const_cast<std::string&>(_value); }

            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitStringExpression(*this);
            }
            explicit StringExpression (const std::string& value) ;
    };

    class NumberExpression : public Expression {
        private:
            float _value;
        public:
            [[nodiscard]] float value() const ;
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitNumberExpression(*this);
            }
            explicit NumberExpression (float value) ;
    };

    class BinaryExpression : public Expression {
        private:
            std::string _operand;
            Expression* _left{};
            Expression* _right{};

        public:
            [[nodiscard]] Expression* left  () const;
            [[nodiscard]] Expression* right () const;

            [[nodiscard]] std::string& operand() const { return const_cast<std::string&>(_operand); }


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

            [[nodiscard]] std::string value() const { return _value; }
            explicit IdentifierExpression (const std::string& value);
            IdentifierExpression () = default;

    };

    class LambdaExpression : public Expression, public CallableExpression {
        private:
            IdentifierExpression _idetifier;
            Expression*          _body;
            std::vector<IdentifierExpression> _args{};

        public:
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitLambdaExpression(*this);
            }
            
            GenericLink call (
                    ICaller* caller, std::stack<Expression*>& stackFrame) override {
                return caller->callLambdaExpression(*this, stackFrame);
            }

            [[nodiscard]] std::vector<IdentifierExpression>& args() const { return const_cast<std::vector<IdentifierExpression>&>(_args); }
            [[nodiscard]] IdentifierExpression& identifier() const { return const_cast<IdentifierExpression&>(_idetifier); }
            [[nodiscard]] Expression*           body () const { return _body; }

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

            [[nodiscard]] Expression* condition() const { return _condition; }
            [[nodiscard]] Expression* then () const { return _then; }
            [[nodiscard]] Expression* els () const { return _else; }

            IfExpression(Expression* condition, Expression* then, Expression* els) 
                : _condition(condition), _then(then), _else(els) {}
    };

    class GroupingExpression : public Expression {
        private:
            Expression* _expression;
        public:
            [[nodiscard]] Expression* expression () const { return _expression; }
        
            loli::GenericLink visit (IVisitor * visitor) override {
                return visitor->visitGroupingExpression(*this);
            }

            explicit GroupingExpression (Expression* expr) : _expression(expr){}
    };

    class BoolExpression : public Expression {
        private:
            bool _value = false;
        public:


            loli::GenericLink visit (IVisitor * visitor) override {
                return visitor->visitBoolExpression(*this);
            }

            [[nodiscard]] bool value () const { return _value; }
            explicit BoolExpression (bool value) : _value(value) {}
    };

    class UnaryExpression : public Expression {
        private:
            std::string _operand;
            NumberExpression *_value;
        
        public:

            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitUnaryExpression(*this);
            }

            [[nodiscard]] float value() const { return _value->value(); }
            [[nodiscard]] std::string operand() const { return _operand; }

            explicit UnaryExpression(const std::string& operand, NumberExpression* value) 
                : _operand(operand), _value(value) {}
    };


    class ClassExpression : public Expression {
        private:
            std::string _name;
            loli::Link<BodyExpression> _body;

        public: 
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitClassExpression(*this);
            }

            [[nodiscard]] std::string name () const { return _name; }
            [[nodiscard]] Link<BodyExpression> body() const { return _body; }

            explicit ClassExpression (
                    const std::string& name,
                    Link<BodyExpression> body) 
                : _name(name), _body(std::move(body)) {}
            explicit ClassExpression (const std::string& name)
                : _name(name) {}
    };



    class BodyExpression : public Expression {
        private:
            std::vector<Expression*>_lines {};

        public:
            std::vector<Expression*> lines () const { return _lines; } 
            
            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitBodyExpression(*this);
            }

            explicit BodyExpression (const std::vector<Expression*>& ls) 
                : _lines (ls){}
    };

    class ForExpression : public Expression {
        private:
            Expression* _firstPart;
            Expression* _condition;
            Expression* _lastPart;
            BodyExpression* _body;

        public:
            Expression* firstPart() const { return _firstPart;}
            Expression* condition() const { return _condition;}
            Expression* lastPart() const { return _lastPart;}
            BodyExpression* body() const { return _body;}

            explicit ForExpression (
                    Expression* firstPart,
                    Expression* condition,
                    Expression* lastPart,
                    BodyExpression* body)
                : 
                    _firstPart((firstPart)), _condition((condition)), 
                    _lastPart((lastPart)), _body(body)
            {}

            GenericLink visit (IVisitor* visitor) override {
                return visitor->visitForExpression(*this);
            }
    };
};
#endif// __EXPRESSION_H__ 
