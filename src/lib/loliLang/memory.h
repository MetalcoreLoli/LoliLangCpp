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
}
#endif // __LOLI_MEMORY__
