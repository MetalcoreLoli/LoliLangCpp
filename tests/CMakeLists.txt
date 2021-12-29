add_executable (unit-tests for_tests.hpp exception_tests.hpp body_tests.hpp ast_tests.hpp class_tests.hpp gtest_main.cpp)
target_link_libraries(unit-tests loliLang pthread gtest)
