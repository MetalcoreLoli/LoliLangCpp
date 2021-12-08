#include <iostream>
#include <string>
#include "./lexer.h"
#include "daphnie.h"
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

    std::shared_ptr<void> visitLambdaExpression (loli::LambdaExpression& value)  override {
        std::string res = "(define (";
        std::string name = *std::static_pointer_cast<std::string>(value.identifier().visit(this));
        std::string body = *std::static_pointer_cast<std::string>(value.body()->visit(this));
        res.append(name);
        if (!value.args().empty()) {
            for (const auto& arg : value.args()) {
                res.append(" ").append(arg.value());
            }
        }   
        res.append (") ").append(body).append(")");
        return std::make_shared<std::string>(res);
    }

    std::shared_ptr<void> visitIdentifierExpression (loli::IdentifierExpression& value) override {
        return std::make_shared<std::string>(value.value());
    }

    std::shared_ptr<void> visitStringExpression (loli::StringExpression& value) override {
        std::string res = "\"";
        res.append (value.value()).append("\"");
        return std::make_shared<std::string>(res);
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

    loli::Daphnie daphnie{tokens};

    auto pp = daphnie.growTree();
    auto result = *std::static_pointer_cast<std::string>(pp->visit(&ast));
    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}
