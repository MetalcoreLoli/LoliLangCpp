#include <cstddef>
#include <iostream>
#include <string>
#include "./lexer.h"
#include "daphnie.h"
#include "expression.h"
#include "lexy.h"
#include "utils.h"

class ASTAsString : public loli::IVisitor {
    loli::GenericLink visitBinaryExpression(loli::BinaryExpression& expression) override {
        auto leftValue  = *std::static_pointer_cast<std::string>(expression.left()->visit(this));
        auto rightValue = *std::static_pointer_cast<std::string>(expression.right()->visit(this));
        std::string result = "(";
        result
            .append(expression.operand()).append(" ")
            .append(leftValue).append(" ")
            .append(rightValue).append(")");
        return loli::newLink<std::string>(result);
    }

    loli::GenericLink visitNumberExpression(loli::NumberExpression& expression) override {
        return loli::newLink<std::string>(std::to_string(expression.value()));
    }

    loli::GenericLink visitLambdaExpression (loli::LambdaExpression& value)  override {
        std::string res = "(define (";
        std::string name = *std::static_pointer_cast<std::string>(value.identifier().visit(this));
        std::string body = *std::static_pointer_cast<std::string>(value.body()->visit(this));
        res.append(name);
        if (!value.args().empty()) {
            for (auto arg = value.args().end() - 1; arg != value.args().begin() - 1; arg--) {
                res.append(" ").append(arg->value());
            }
        }   
        res.append (") ").append(body).append(")");
        return loli::newLink<std::string>(res);
    }

    loli::GenericLink visitIdentifierExpression (loli::IdentifierExpression& value) override {
        return loli::newLink<std::string>(value.value());
    }

    loli::GenericLink visitStringExpression (loli::StringExpression& value) override {
        std::string res = "\"";
        res.append (value.value()).append("\"");
        return loli::newLink<std::string>(res);
    }
};


int main () {
    loli::Lexer lex;
    std::string code = 
        "someFunc n => n*6 + 5 / 99 -9;";
    
    std::cout << code << std::endl;
    std::cout << std::string(35, '-') << std::endl;

    auto tokens = lex.lineToTokens (code);
    for (const auto& token : tokens) {
        std::cout << token.asString() << std::endl;
    }
    std::cout << std::string(35, '-') << std::endl;
    
    ASTAsString ast;

    loli::Daphnie daphnie{tokens};

    auto result = loli::unwarp<void, std::string>(daphnie.growTree()->visit(&ast));
    std::cout << result << std::endl;
    return EXIT_SUCCESS;
}
