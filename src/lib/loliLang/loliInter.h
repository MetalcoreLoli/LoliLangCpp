#ifndef __LOLI_INTER__
#define __LOLI_INTER__


#include <stack>
#include <string_view>

#include "lexy.h"
#include "utils.h"
#include "lexer.h"
#include "daphnie.h"
#include "expression.h"

namespace loli {
    class LoliInter {
        public:
            utils::GenericLink AnswerOn (std::string_view sv);
    };
};
#endif // __LOLI_INTER__
