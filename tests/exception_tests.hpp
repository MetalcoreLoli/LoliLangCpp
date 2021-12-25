#ifndef __LOLI_EXCEPTIONS_TESTS__
#define __LOLI_EXCEPTIONS_TESTS__

#include "loliLang/exceptions.h"
#include <exception>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include <loliLang/common.h>

TEST (SyntaxErrorExceptionTests, SyntaxErrorException_WithForma_ThrowsWithDefaultMessage) {
    try {
        throw loli::SyntaxErrorException(loli::Forma::WITH);
    } catch (const std::exception& ex) {
        ASSERT_STREQ(ex.what(), "There is syntax error near `WITH`");
    }
}

#endif // __LOLI_EXCEPTIONS_TESTS__
