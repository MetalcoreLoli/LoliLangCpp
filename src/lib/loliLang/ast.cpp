#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>

#include "./lexer.h"
#include "daphnie.h"
#include "expression.h"
#include "lexy.h"
#include "utils.h"

namespace loli {
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
    
        loli::GenericLink visitIfExpression (loli::IfExpression& value) override {
            std::string res = "(if ";
            auto condition = loli::unwrap<void, std::string>(value.condition()->visit(this));
            auto then      = loli::unwrap<void, std::string>(value.then()->visit(this));
            auto els       = loli::unwrap<void, std::string>(value.els()->visit(this));
    
            res
                .append(condition).append(" ")
                .append(then)     .append(" ")
                .append(els)      .append(")");
            return loli::newLink<std::string> (res);
        }
    
        loli::GenericLink visitGroupingExpression (loli::GroupingExpression& value) override {
            return value.expression()->visit(this);
        }
        
        loli::GenericLink visitBoolExpression (loli::BoolExpression& value) override {
            return loli::newLink<std::string>(std::to_string(value.value()));
        }

        loli::GenericLink visitUnaryExpression (loli::UnaryExpression& value) override {
            std::string res = "(" + value.operand()+" ";
            res.append(std::to_string(value.value())).append(")");
            return loli::newLink<std::string>(res);
        }

        loli::GenericLink visitClassExpression (loli::ClassExpression& value) override {
            std::string res = "(class ("+value.name()+")";

            if (value.body() != nullptr) {
                //properties
                for (auto& property: value.body()->lines()) {
                    res.append(" ").append(loli::unwrap<void, std::string>(property->visit(this)));
                }
            }

            return loli::newLink<std::string>(res.append(")"));
        }

        loli::GenericLink visitBodyExpression (loli::BodyExpression& value) override {
            std::string res = "(with";

            for (auto line : value.lines()) {
                res.append(" ").append(loli::unwrap<void, std::string>(line->visit(this)));
            }

            return loli::newLink<std::string>(res.append(")"));
        }

        loli::GenericLink  visitForExpression (loli::ForExpression& value) override {
            std::string res = "(for ";
            res
                .append(loli::unwrap<void, std::string>(value.firstPart()->visit(this)))
                .append(" ")
                .append(loli::unwrap<void, std::string>(value.condition()->visit(this)))
                .append(" ")
                .append(loli::unwrap<void, std::string>(value.lastPart()->visit(this)))
                .append(" ")
                .append(loli::unwrap<void, std::string> (value.body()->visit(this)))
                .append(")");
            return loli::newLink<std::string>(res);
        }
        
        loli::GenericLink  visitCallExpression(loli::CallExpression& value) override {
            std::string res = "(call ";
            res
                .append(loli::unwrap<void, std::string>(value.idetifier().visit(this)));
            for (auto& arg : value.args()) {
                res.append(" ").append(loli::unwrap<void, std::string> (arg->visit(this)));
            }

            res.append(")");
            return loli::newLink<std::string>(res);
        }



    };
}
