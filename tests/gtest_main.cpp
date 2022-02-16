#include "ast_tests.hpp"
#include "class_tests.hpp"
#include "body_tests.hpp"
#include "for_tests.hpp"
#include "exception_tests.hpp"
#include "loli_tests.hpp"
#include "lexy_tests.hpp"
#include "call_tests.hpp"
#include "typechecking_tests.hpp"
#include "typemethods_tests.hpp"
#include "lexer_tests.hpp"
#include "daphnie_tests.hpp"

int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
