#ifndef LOLI_UTILS_H
#define LOLI_UTILS_H

#include <memory>

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

        bool HasValue() const {
            return _value != nullptr;            
        }


        Maybe(T value) {
            Value (value);
        };
    };

    template<typename T>
    using Link = std::shared_ptr<T>;
    
    template<typename T, typename ... Args>
    constexpr Link<T> newLink(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using GenericLink = Link<void>;
};
#endif
