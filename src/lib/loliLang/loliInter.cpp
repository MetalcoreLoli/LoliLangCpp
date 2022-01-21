#include "loliInter.h"
#include "loliLang/expression.h"
#include "loliLang/lexy.h"



loli::LoliInter::ResultOfAnswering loli::LoliInter::AnswerOn (std::string_view sv) {
    loli::Daphnie d {std::string(sv)};
    auto expression = d.growTree();
    return ResultOfAnswering{expression->visit(&_lexy)};
}
