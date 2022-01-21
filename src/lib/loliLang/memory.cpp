
#include "memory.h"
#include "loliLang/utils.h"

loli::IMemoryOf<loli::Expression*>& loli::MemoryTableOfExpressions::Push (Expression* value) {
    _mainStack.push_back(value);
    return *this;
} 

bool loli::MemoryTableOfExpressions::TryFind (utils::Spec<Expression*>* spec, Expression** out)  {
    for (Expression* v : _mainStack) {
        if (spec->IsSatisfy(v)) {
            *out = v; 
            return true;
        }
    }
    return false;
}
