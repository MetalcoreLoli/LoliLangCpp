#include  "exceptions.h"

const char* loli::ElseBranchException::what() const noexcept {
    return _message.c_str();
}

const char* loli::SyntaxErrorException::what() const noexcept {
    return _message.c_str();
}
