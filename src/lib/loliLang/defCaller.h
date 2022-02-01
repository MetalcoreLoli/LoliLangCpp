#ifndef __LOLI_DEFCALLER__
#define __LOLI_DEFCALLER__

#include "loliLang/lexy.h"
#include "loliLang/expression.h"

#include <memory>

namespace loli {
    struct DefCaller : public loli::ICaller {
        ReturnResult
        callLambdaExpression(
                LambdaExpression& value, 
                std::stack<Expression*>& stackFrame) override;
    };
};

#endif //__LOLI_DEFCALLER__
