#ifndef __LOLI_MEMORY__
#define __LOLI_MEMORY__

#include "loliLang/expression.h"
#include "utils.h"

namespace loli {
    template <typename T>
    struct IMemoryOf {
        virtual IMemoryOf<T>& Push (T value) = 0;
        virtual bool TryFind (utils::Spec<T>* spec, T* out) = 0; 
    };

    class MemoryTableOfExpressions : public IMemoryOf<Expression*> {
        std::vector <Expression*> _mainStack {};

        public:
            void Clean () {_mainStack.clear();} 
            IMemoryOf<Expression*>& Push (Expression* value) override;
            bool TryFind (utils::Spec<Expression*>* spec, Expression** out) override; 
    };

    namespace mem {
        class Environment {
            using SpecFilter = loli::Link<utils::Spec<Expression*>>;
            std::vector<Expression*> _mainStack{};


            Environment() {}
            public:
                Environment(Environment&) = delete;
                void Push(Expression* value) {
                    _mainStack.push_back(value);
                }

                bool TryFind (const SpecFilter spec, Expression** out) {
                   for (auto expr : _mainStack) {
                       if (spec->IsSatisfy(expr)) {
                           *out = expr;
                           return true;
                       }
                   } 
                   return false;
                }

                void Clear () {
                    _mainStack.clear();
                }

                static Environment& Instance() {
                    static Environment _ptr; 
                    return _ptr;
                }
        };

    };
}
#endif // __LOLI_MEMORY__
