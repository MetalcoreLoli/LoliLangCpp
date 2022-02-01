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
                ReturnResult _rawResult;

                public:
                    ReturnResult& RawResult () const { 
                        return const_cast<ReturnResult&>(_rawResult); 
                    };

                    template<typename TResultAs> 
                    TResultAs As() {
                        return (_rawResult).Unwrap<TResultAs>();
                    }

                    explicit ResultOfAnswering (const ReturnResult& result)
                        : _rawResult(result) {}
            };

            ResultOfAnswering AnswerOn (std::string_view sv);
    };
};
#endif // __LOLI_INTER__
