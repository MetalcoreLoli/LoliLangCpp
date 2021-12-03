#include <iostream>
#include <string>
#include "./lexer.h"
#include "expression.h"
#include "lexy.h"

class ASTAsString : public loli::IVisitor {
    std::shared_ptr<void> visitBinaryExpression(loli::BinaryExpression& expression) override {
        auto leftValue  = *std::static_pointer_cast<std::string>(expression.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<std::string>(expression.right()->visit(this));
        std::string result = "(";
        result
            .append(expression.operand())
            .append(" ")
            .append(leftValue)
            .append(" ")
            .append(rightValue).append(")");
        return std::make_shared<std::string>(result);
    }

    std::shared_ptr<void> visitNumberExpression(loli::NumberExpression& expression) override {
        return std::make_shared<std::string>(std::to_string(expression.value()));
    }
};


int main () {
    loli::Lexer lex;
    std::string code = 
        "helloWorld => \"Hello, world\";";
    auto tokens = lex.lineToTokens (code);
    for (const auto& token : tokens) {
        std::cout << token.asString() << std::endl;
    }
    ASTAsString ast;

    loli::BinaryExpression cc("+", 
            new loli::BinaryExpression("-", new loli::NumberExpression(10.0f),  new loli::NumberExpression(14.0f)), 
            new loli::BinaryExpression("*", new loli::NumberExpression(5.0f),   new loli::NumberExpression(5.0f)));

    auto res = *static_cast<std::string*>(cc.visit(&ast).get());
    std::cout << res << std::endl;

    loli::Lexy lexy;
    auto result = *std::static_pointer_cast<float>(lexy.visitBinaryExpression(cc));
    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}
