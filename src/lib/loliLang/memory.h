#ifndef __LOLI_MEMORY__
#define __LOLI_MEMORY__

#include "loliLang/expression.h"
#include "utils.h"

namespace loli {
    template <typename T>
    struct IMemoryOf {
        virtual IMemoryOf<T>& Push (T value) = 0;
        virtual bool TryFind (utils::Spec<T>* spec, T* out) = 0; 
    };

    class MemoryTableOfExpressions : public IMemoryOf<Expression*> {
        std::vector <Expression*> _mainStack {};

        public:
            void Clean () {_mainStack.clear();} 
            IMemoryOf<Expression*>& Push (Expression* value) override;
            bool TryFind (utils::Spec<Expression*>* spec, Expression** out) override; 
    };

    namespace mem {
        using ExpressionFilter = loli::Link<utils::Spec<Expression*>>;
        struct IEnvironment {
            virtual IEnvironment& Push (Expression*) =0;
            virtual IEnvironment& Clear() =0;
            virtual bool TryFind (const ExpressionFilter spec, Expression **out) = 0;
        };
        
        class LocalEnvironment : public IEnvironment {
            std::vector<Expression*> _container {};
            public:
                ~LocalEnvironment () {Clear();}
                IEnvironment& Push(Expression* value) {
                    _container.push_back(value);
                    return *this;
                }

                bool TryFind (const ExpressionFilter spec, Expression** out) {
                   for (auto expr : _container) {
                       if (spec->IsSatisfy(expr)) {
                           *out = expr;
                           return true;
                       }
                   } 
                   return false;
                }

                IEnvironment& Clear () {
                    _container.clear();
                    return *this;
                }
        };

        class Environment : public IEnvironment {
            std::vector<Expression*> _mainStack{};

            Environment() {}
            public:
                Environment(Environment&) = delete;

                IEnvironment& Push(Expression* value) {
                    _mainStack.push_back(value);
                    return *this;
                }

                bool TryFind (const ExpressionFilter spec, Expression** out) {
                   for (auto expr : _mainStack) {
                       if (spec->IsSatisfy(expr)) {
                           *out = expr;
                           return true;
                       }
                   } 
                   return false;
                }

                IEnvironment& Clear () {
                    _mainStack.clear();
                    return *this;
                }

                static Environment& Instance() {
                    static Environment _ptr; 
                    return _ptr;
                }
        };

        static Environment& GlobalEnvironment = Environment::Instance(); 

    };
}
#endif // __LOLI_MEMORY__
