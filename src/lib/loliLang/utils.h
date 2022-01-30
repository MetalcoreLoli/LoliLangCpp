#ifndef LOLI_UTILS_H
#define LOLI_UTILS_H

#include <memory>

#include <string>
#include <string_view>
#include <type_traits>
namespace loli::utils {
    template <typename T>
        requires std::is_pointer_v<T>
    class Maybe {
    private:
        T _value;
    public:
        T Value() const { return _value; }
        Maybe<T>& Value(const T value) {
            if (_value != value)
                _value = value;
            return *this;
        }

        [[nodiscard]] bool HasValue() const {
            return _value != nullptr;            
        }

        explicit Maybe(T value) {
            Value (value);
        };
    };

    template<typename T>
    using Link = std::shared_ptr<T>;
    
    template<typename T>
    using UniLink = std::unique_ptr<T>;
    
    using GenericLink = Link<void>;
    using RawGenericLink = void*;

    template<typename T, typename ... Args>
    constexpr Link<T> newLink(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    constexpr Link<T> newUniLink(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename TFrom, typename TTo>
    constexpr TTo unwrap (Link<TFrom> link) {
        return *std::static_pointer_cast<TTo> (link);
    }

    struct ThrowHelper {
        static void Throw_NotImplemented (const std::string& methodName) {
            throw std::runtime_error {methodName + " is not impelented yet."};     
        }
        static void Throw_ThereIsNo (const std::string& value) {
            throw std::runtime_error {"There is no `" + value + "` word"};     
        }
    };


    template <typename T>
    struct AndSpec;
    
    template <typename T>
    struct OrSpec;

    template <typename T>
    struct Spec {
        virtual bool IsSatisfy (T item) = 0;

        inline AndSpec<T> operator && (Spec<T> b) {
            return AndSpec<T>(newLink <Spec<T>>(*this), newLink <Spec<T>>(b));
        }

        inline OrSpec<T> operator || (Spec<T> b) {
            return OrSpec<T>(newLink <Spec<T>>(*this), newLink <Spec<T>>(b));
        }
    };

    template <typename T>
    struct AndSpec : public Spec<T> {
        bool IsSatify (T item) {
            return _left->IsSatisfy (item) && _right->IsSatisfy(item);
        }
        AndSpec (Link<Spec<T>>& left, Link<Spec<T>>& right) 
            : _left (left), _right (right) {

        }
        private:
            Link<Spec<T>> _left;
            Link<Spec<T>> _right;
    };
    
    template <typename T>
    struct OrSpec: public Spec<T> {
        bool IsSatify (T item) {
            return _left->IsSatisfy (item) && _right->IsSatisfy(item);
        }
        OrSpec (Link<Spec<T>>& left, Link<Spec<T>>& right) 
            : _left (left), _right (right) {

        }
        private:
            Link<Spec<T>> _left;
            Link<Spec<T>> _right;
    };

};
#endif
