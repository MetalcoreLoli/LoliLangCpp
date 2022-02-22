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
        loli::ReturnResult visitBinaryExpression(loli::BinaryExpression& expression) override {
            auto leftValue  = (expression.left()->visit(this)).Unwrap<std::string>();
            auto rightValue = (expression.right()->visit(this)).Unwrap<std::string>();
            std::string result = "(";
            result
                .append(expression.operand()).append(" ")
                .append(leftValue).append(" ")
                .append(rightValue).append(")");
            return {loli::newLink<std::string>(result), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitNumberExpression(loli::NumberExpression& expression) override {
            return {loli::newLink<std::string> (std::to_string(expression.value())), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitLambdaExpression (loli::LambdaExpression& value)  override {
            std::string res = "(define (";
            std::string name = (value.identifier().visit(this)).Unwrap<std::string>();
            std::string body = (value.body()->visit(this)).Unwrap<std::string>();
            res.append(name);
            if (!value.args().empty()) {
                for (auto arg = value.args().end() - 1; arg != value.args().begin() - 1; arg--) {
                    res.append(" ").append(arg->value());
                }
            }   
            res.append (") ").append(body).append(")");
            return {loli::newLink<std::string> (res), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitIdentifierExpression (loli::IdentifierExpression& value) override {
            return {loli::newLink<std::string> (value.value()), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitStringExpression (loli::StringExpression& value) override {
            std::string res = "\"";
            res.append (value.value()).append("\"");
            return {loli::newLink<std::string> (res), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitIfExpression (loli::IfExpression& value) override {
            std::string res = "(if ";
            auto condition = (value.condition()->visit(this)).Unwrap<std::string>();
            auto then      = (value.then()->visit(this)).Unwrap<std::string>();
            auto els       = (value.els()->visit(this)).Unwrap<std::string>();
    
            res
                .append(condition).append(" ")
                .append(then)     .append(" ")
                .append(els)      .append(")");
            return {loli::newLink<std::string> (res), typeid(std::string).hash_code()};
        }
    
        loli::ReturnResult visitGroupingExpression (loli::GroupingExpression& value) override {
            return value.expression()->visit(this);
        }
        
        loli::ReturnResult visitBoolExpression (loli::BoolExpression& value) override {
            return {loli::newLink<std::string>(std::to_string(value.value())), typeid(std::string).hash_code()};
        }

        loli::ReturnResult visitUnaryExpression (loli::UnaryExpression& value) override {
            std::string res = "(" + value.operand()+" ";
            res.append(std::to_string(value.value())).append(")");
            return {loli::newLink<std::string>(res), typeid(std::string).hash_code()};
        }

        loli::ReturnResult visitClassExpression (loli::ClassExpression& value) override {
            std::string res = "(class ("+value.name()+")";

            if (value.body() != nullptr) {
                //properties
                for (auto& property: value.body()->lines()) {
                    res.append(" ").append((property->visit(this)).Unwrap<std::string>());
                }
            }

            return {loli::newLink<std::string>(res.append(")")), typeid(std::string).hash_code()};
        }

        loli::ReturnResult visitBodyExpression (loli::BodyExpression& value) override {
            std::string res = "(with";

            for (auto line : value.lines()) {
                res.append(" ").append((line->visit(this)).Unwrap<std::string>());
            }

            return {loli::newLink<std::string>(res.append(")")), typeid(std::string).hash_code()};
        }

        loli::ReturnResult  visitForExpression (loli::ForExpression& value) override {
            std::string res = "(for ";
            res
                .append((value.firstPart()->visit(this)).Unwrap<std::string>())
                .append(" ")
                .append((value.condition()->visit(this)).Unwrap<std::string>())
                .append(" ")
                .append((value.lastPart()->visit(this)).Unwrap<std::string>())
                .append(" ")
                .append( (value.body()->visit(this)).Unwrap<std::string>())
                .append(")");
            return {loli::newLink<std::string>(res), typeid(std::string).hash_code()};
        }

        loli::ReturnResult visitWhereExpression (WhereExpression& value) override {
            std::string res = "(where ";
            res.append(value.func()->visit(this).Unwrap<std::string>()).append(" (");
            for (auto arg : value.args()) {
                res.append(" ").append(arg->visit(this).Unwrap<std::string>());
            }
            return loli::ReturnResult::New(res.append(")"));
        }
        
        loli::ReturnResult  visitCallExpression(loli::CallExpression& value) override {
            std::string res = "(call ";
            res
                .append((value.idetifier().visit(this)).Unwrap<std::string>());
            for (Expression* arg : value.args()) {
                res.append(" ").append((arg->visit(this)).Unwrap<std::string>());
            }

            res.append(")");
            return {loli::newLink<std::string>(res), typeid(std::string).hash_code()};
        }
    };
}
