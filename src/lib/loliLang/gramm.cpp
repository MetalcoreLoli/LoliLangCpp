#include "gramm.h"

bool loli::DefGrammarChecker::FindTokenWithForma    (size_t start, loli::Forma forma){
    if (start > _source.size()) return false;

    for (size_t i = start; i < _source.size(); i++) {
        if (_source[i].forma() == forma) return true;
    }
    throw loli::SyntaxErrorException (forma);
} 

bool loli::DefGrammarChecker::TryFindTokenWithForma (size_t start, loli::Forma forma)  {
    if (start > _source.size()) return false;

    for (size_t i = start; i < _source.size(); i++) {
        if (_source[i].forma() == forma) return true;
    }

    return false;
}

void loli::DefGrammarChecker::TryFindTokenWithFormaOrThrow (size_t start, loli::Forma forma, std::string_view message)  {
    if (!TryFindTokenWithForma(start, forma)) {
        throw loli::SyntaxErrorException(message);
    }
}
