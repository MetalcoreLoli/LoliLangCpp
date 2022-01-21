
#include "memory.h"
#include "loliLang/utils.h"

loli::IMemoryOf<loli::Expression*>& loli::MemoryTableOfExpressions::Push (Expression* value) {
    utils::ThrowHelper::Throw_NotImplemented("loli::MemoryTableOfExpressions::Push");
} 

bool loli::MemoryTableOfExpressions::TryFind (utils::Spec<Expression*>* spec, Expression** out)  {
    utils::ThrowHelper::Throw_NotImplemented("loli::MemoryTableOfExpressions::TryFind");
}
