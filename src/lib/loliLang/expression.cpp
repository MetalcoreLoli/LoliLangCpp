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

loli::IdentifierExpression::IdentifierExpression(const std::string& value) {
    _value = value;
}

loli::LambdaExpression::LambdaExpression(loli::IdentifierExpression& identifier, loli::Expression* body) {
    _idetifier = identifier;
    _body  = body;
}

loli::LambdaExpression::LambdaExpression(
        loli::IdentifierExpression& identifier, 
        std::vector<IdentifierExpression>& args, 
        loli::Expression* body) {
    _idetifier = identifier;
    _args = args;
    _body = body;

}
loli::StringExpression::StringExpression(const std::string& value)  {
    _value = value;
}

bool loli::LambdaExpressionTypeSpec::IsSatisfy (loli::Expression* item) {
    size_t hashCode = typeid(*item).hash_code();
    return hashCode == this->hashCode;
}

bool loli::LambdaExpressionNameSpec::IsSatisfy (loli::LambdaExpression& item) {
    return item.identifier().value() == this->_name;
}
