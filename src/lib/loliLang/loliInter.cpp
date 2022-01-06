#include "loliInter.h"
#include "loliLang/lexy.h"



loli::LoliInter::ResultOfAnswering loli::LoliInter::AnswerOn (std::string_view sv) {
    loli::Lexy lexy{};
    loli::Daphnie d {std::string(sv)};

    return ResultOfAnswering{d.growTree()->visit(&lexy)};
}
