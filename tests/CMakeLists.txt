add_executable (unit-tests body_tests.hpp ast_tests.hpp class_tests.hpp gtest_main.cpp)
target_link_libraries(unit-tests loliLang pthread gtest)
