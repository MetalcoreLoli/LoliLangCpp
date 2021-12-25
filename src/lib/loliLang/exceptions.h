#ifndef __LOLI__EXCEPTIONS__
#define __LOLI__EXCEPTIONS__

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

#include "lexer.h"

namespace loli {
    class ElseBranchException : public std::exception {
        private: 
            std::string _message;

        public:
            explicit ElseBranchException (std::string  message) : _message(std::move(message)){}
            ElseBranchException() : _message ("There is no 'else' branch") {}

            virtual const char* what() const noexcept override;
    };

    class SyntaxErrorException : public std::exception {
        private: 
            std::string _message;

        public:
            explicit SyntaxErrorException (std::string  message) : _message(std::move(message)){}
            SyntaxErrorException(loli::Forma forma) 
                : _message ("There is syntax error near `" + std::string(loli::FormaAsStr[(size_t)forma]) + "`") 
            {}

            virtual const char* what() const noexcept override;

    };
}
#endif // __LOLI__EXCEPTIONS__
