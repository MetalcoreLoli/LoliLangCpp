#include "./expression.h"



loli::Expression* loli::BinaryExpression::left  () const {
    return _left;
}

loli::Expression* loli::BinaryExpression::right () const {
    return _right;
}

loli::BinaryExpression& loli::BinaryExpression::left  (loli::Expression* value) {
    _left = value;
    return *this;
}

loli::BinaryExpression& loli::BinaryExpression::right (loli::Expression* value){
    _right = value;
    return *this;
}

loli::BinaryExpression::BinaryExpression(const std::string& operand, loli::Expression* leftValue, loli::Expression* rightValue) {
    left(leftValue).right(rightValue); 
    _operand = operand;
}



loli::NumberExpression::NumberExpression(float value)  {
    _value = value;
}
float loli::NumberExpression::value() const { return _value; }
