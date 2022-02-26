#ifndef __LOLI_CALL__
#define __LOLI_CALL__
#include "loliLang/expression.h"
#include "loliLang/memory.h"
#include "lexy.h"
#include <string>

namespace loli {
    class Call {
        std::vector<LambdaExpression*> _mappedArgs{};
        std::vector<Expression*> _callArgs{};
        mem::IEnvironment *_globalEnv;
        mem::IEnvironment *_localEnv;
        loli::Lexy _local;

        loli::LambdaExpression _lambda = *ExpressionFactory::LambdaRaw("", nullptr);
        public:
            Call(mem::IEnvironment* env) : _globalEnv(env), _localEnv(new mem::LocalEnvironment){
                _local = {_globalEnv};
            }
            Call(mem::IEnvironment* env, mem::IEnvironment* local) : _globalEnv(env) , _localEnv(local){
                _local = {_globalEnv};
            }
            Call& Validate (const loli::CallExpression& value, mem::IEnvironment* local);
            Call& Map ();
            Call& FillLocalStackFrame (loli::Lexy& global);
            loli::ReturnResult Execute();

            static Call Create(mem::IEnvironment* env) {return {env};}
            static Call Create(mem::IEnvironment* env,mem::IEnvironment* localEnv) {return {env, localEnv};}
    };
}

#endif //__LOLI_CALL__
