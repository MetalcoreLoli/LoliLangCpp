#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <any>
#include <string>
#include <memory>
#include <vector>

namespace loli {
    class Expression;
    class IdentifierExpression;
    class NumberExpression;
    class BinaryExpression;
    class LambdaExpression;
    class StringExpression;
    
    struct IVisitor {
        virtual std::shared_ptr<void> visitBinaryExpression (BinaryExpression& value) = 0;
        virtual std::shared_ptr<void> visitNumberExpression (NumberExpression& value) = 0;
        virtual std::shared_ptr<void> visitLambdaExpression (LambdaExpression& value) = 0;
        virtual std::shared_ptr<void> visitIdentifierExpression (IdentifierExpression& value) = 0;
        virtual std::shared_ptr<void> visitStringExpression (StringExpression& value) = 0;
    };

    struct Expression {
        virtual std::shared_ptr<void> visit (IVisitor * visitor) = 0;
    };

    class StringExpression : public Expression {
        private:
            std::string _value;
        public:
            std::string& value() const { return const_cast<std::string&>(_value); }

            std::shared_ptr<void> visit (IVisitor* visitor) override {
                return visitor->visitStringExpression(*this);
            }
            explicit StringExpression (const std::string& value) ;
    };

    class NumberExpression : public Expression {
        private:
            float _value;
        public:
            float value() const ;
            std::shared_ptr<void> visit (IVisitor* visitor) override {
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

            std::shared_ptr<void> visit (IVisitor* visitor) override {
                return visitor->visitBinaryExpression(*this);
            }

            BinaryExpression (const std::string& operand, Expression* left, Expression* right);
    };

    class IdentifierExpression : public Expression {
        private:
            std::string _value;

        public:
            std::shared_ptr<void> visit (IVisitor* visitor) override {
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
            std::shared_ptr<void> visit (IVisitor* visitor) override {
                return visitor->visitLambdaExpression(*this);
            }

            std::vector<IdentifierExpression>& args() const { return const_cast<std::vector<IdentifierExpression>&>(_args); }
            IdentifierExpression& identifier() const { return const_cast<IdentifierExpression&>(_idetifier); }
            Expression*           body () const { return _body; }

            LambdaExpression(IdentifierExpression& identifier, Expression* body);
            LambdaExpression(IdentifierExpression& identifier, std::vector<IdentifierExpression>& args, Expression* body);
    };
};
#endif// __EXPRESSION_H__ 
