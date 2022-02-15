#ifndef __LOLI_INTER__
#define __LOLI_INTER__


#include <stack>
#include <string_view>

#include "lexy.h"
#include "loliLang/memory.h"
#include "utils.h"
#include "daphnie.h"
#include "ast.cpp"
namespace loli {
    struct IWriter {
        virtual void Write (const std::string& message) = 0;
    };

    class LoliInter {
        private:
            mem::LocalEnvironment* _global = new mem::LocalEnvironment;
            loli::Lexy _lexy{};
        public:
            LoliInter () {
            }
            
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
            ResultOfAnswering AnswerOn (std::string_view sv, IWriter* writer);
    };
};
#endif // __LOLI_INTER__
