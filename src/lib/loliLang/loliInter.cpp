#include "loliInter.h"
#include "loliLang/lexy.h"



loli::utils::GenericLink loli::LoliInter::AnswerOn (std::string_view sv) {
    loli::Lexy lexy{};
    loli::Daphnie d {std::string(sv)};

    return d.growTree()->visit(&lexy);
}
