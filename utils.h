#ifndef LOLI_UTILS_H
#define LOLI_UTILS_H

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
};
#endif
