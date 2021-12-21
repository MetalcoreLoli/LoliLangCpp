#ifndef __LOLI__EXCEPTIONS__
#define __LOLI__EXCEPTIONS__

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>
namespace loli {
    class ElseBranchException : public std::exception {
        private: 
            std::string _message;

        public:
            explicit ElseBranchException (std::string  message) : _message(std::move(message)){}
            ElseBranchException() : _message ("There is no 'else' branch") {}

            virtual const char* what() const noexcept override;
    };
}
#endif // __LOLI__EXCEPTIONS__
