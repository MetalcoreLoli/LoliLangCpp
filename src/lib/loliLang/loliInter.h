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
        private:
            loli::Lexy _lexy{};
        public:
            
            class ResultOfAnswering {
                utils::GenericLink _rawResult;

                public:
                    utils::GenericLink& RawResult () const { 
                        return const_cast<utils::GenericLink&>(_rawResult); 
                    };

                    template<typename TResultAs> 
                    TResultAs As() {
                        return loli::unwrap<void, TResultAs>(_rawResult);
                    }

                    explicit ResultOfAnswering (const utils::GenericLink& result)
                        : _rawResult(result) {}
            };

            ResultOfAnswering AnswerOn (std::string_view sv);
    };
};
#endif // __LOLI_INTER__
