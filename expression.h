#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <any>
#include <string>
#include <memory>

namespace loli {
    class Expression;
    class NumberExpression;
    class BinaryExpression;
    
    struct IVisitor {
        virtual std::shared_ptr<void> visitBinaryExpression (BinaryExpression& value) = 0;
        virtual std::shared_ptr<void> visitNumberExpression (NumberExpression& value) = 0;
    };


    struct Expression {
        virtual std::shared_ptr<void> visit (IVisitor * visitor) = 0;
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
};
#endif// __EXPRESSION_H__ 
