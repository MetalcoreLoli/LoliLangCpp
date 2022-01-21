#ifndef __LEXY_H__
#define __LEXY_H__

#include <map>
#include <string>
#include <functional>


#include "expression.h"
#include "loliLang/memory.h"
#include "loliLang/utils.h"

namespace loli {
    struct Lexy : IVisitor {
        GenericLink visitBinaryExpression (BinaryExpression& value) override;
        GenericLink visitNumberExpression (NumberExpression& value) override;
        GenericLink visitLambdaExpression (LambdaExpression& value) override;
        GenericLink visitIdentifierExpression (IdentifierExpression& value) override;
        GenericLink visitStringExpression (StringExpression& value) override;
        GenericLink visitIfExpression (IfExpression& value) override;
        GenericLink visitGroupingExpression (GroupingExpression& value) override;
        GenericLink visitBoolExpression (BoolExpression& value) override;
        GenericLink visitUnaryExpression (UnaryExpression& value) override;
        GenericLink visitClassExpression (ClassExpression& value) override;
        GenericLink visitBodyExpression (BodyExpression& value) override;
        GenericLink visitForExpression (ForExpression& value) override;

        Lexy& PushIntoMainStack (Expression* expression);

        explicit Lexy () {
        }
        private:

            loli::IMemoryOf<Expression*>* _memory = new loli::MemoryTableOfExpressions;
            std::vector <Expression*> _mainStack {};
            //{"func", Token(Forma::FUNC, "func", 0)},
            std::map<std::string, std::function<GenericLink(float, float)>> _opsTable{
                {"+", [](float a, float b) -> GenericLink {return loli::newLink<float>(a+b);}},
                {"-", [](float a, float b) -> GenericLink {return loli::newLink<float>(a-b);}},
                {"*", [](float a, float b) -> GenericLink {return loli::newLink<float>(a*b);}},
                {"/", [](float a, float b) -> GenericLink {return loli::newLink<float>(a/b);}},
                {">", [](float a, float b) -> GenericLink {return loli::newLink<bool>(a<b);}},
                {"<", [](float a, float b) -> GenericLink {return loli::newLink<bool>(a>b);}}
            };

    };
};
#endif // __LEXY_H__
