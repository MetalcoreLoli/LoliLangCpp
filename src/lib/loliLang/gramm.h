#ifndef __LOLI_GRAMM__
#define __LOLI_GRAMM__

#include <vector>

#include "token.h"
#include "exceptions.h"

namespace loli {
    struct IGrammarChecker {
        virtual bool FindTokenWithForma (size_t start, loli::Forma forma) = 0;
        virtual bool TryFindTokenWithForma (size_t start, loli::Forma forma) = 0;

        virtual void TryFindTokenWithFormaOrThrow (size_t start, loli::Forma forma, std::string_view message) = 0;
    };

    struct DefGrammarChecker : public IGrammarChecker {

        bool FindTokenWithForma (size_t start, loli::Forma forma) override;
        bool TryFindTokenWithForma (size_t start, loli::Forma forma) override;
        void TryFindTokenWithFormaOrThrow (size_t start, loli::Forma forma, std::string_view message) override;

        explicit DefGrammarChecker (const std::vector<Token>& code) : _source (code){}
        
        private:
            std::vector<Token> _source;
    };
};

#endif // __LOLI_GRAMM__
