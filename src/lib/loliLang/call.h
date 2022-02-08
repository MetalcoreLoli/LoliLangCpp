#ifndef __LOLI_CALL__
#define __LOLI_CALL__
#include "loliLang/expression.h"
#include "loliLang/memory.h"
#include "lexy.h"
#include <string>

namespace loli {
    class Call {
        loli::Lexy _local{};
        std::vector<LambdaExpression*> _mappedArgs{};
        std::vector<Expression*> _callArgs{};
        mem::IEnvironment *_globalEnv;

        loli::LambdaExpression _lambda = *ExpressionFactory::LambdaRaw("", nullptr);
        public:
            Call(mem::IEnvironment* env) : _globalEnv(env) {}
            Call& Validate (const loli::CallExpression& value);
            Call& Map ();
            Call& FillLocalStackFrame (loli::Lexy& global);
            loli::ReturnResult Execute();

            static Call Create(mem::IEnvironment* env) {return {env};}
    };
}

#endif //__LOLI_CALL__
