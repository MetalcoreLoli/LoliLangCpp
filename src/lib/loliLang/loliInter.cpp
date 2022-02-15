#include "loliInter.h"
#include "loliLang/expression.h"
#include "loliLang/lexy.h"
#include <cstring>
#include <string>



loli::LoliInter::ResultOfAnswering loli::LoliInter::AnswerOn (std::string_view sv) {
    return AnswerOn(sv, nullptr);
}

loli::LoliInter::ResultOfAnswering loli::LoliInter::AnswerOn (std::string_view sv, IWriter* writer) {
    loli::Daphnie d {std::string(sv)};
    auto expression = d.growTree();
    if (writer != nullptr) {
        loli::ASTAsString ast{};
        writer->Write(expression->visit(&ast).Unwrap<std::string>());
    }
    return ResultOfAnswering{expression->visit(&_lexy)};
}
