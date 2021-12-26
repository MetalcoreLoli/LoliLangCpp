#ifndef __LOLI_GRAMM__
#define __LOLI_GRAMM__

#include "token.h"

namespace loli {
    struct IGrammarChecker {
        virtual bool FindTokenWithForma (size_t start, loli::Forma forma) = 0;
        virtual bool TryFindTokenWithForma (size_t start, loli::Forma forma) = 0;
    };
};

#endif // __LOLI_GRAMM__
