cmake_minimum_required(VERSION 3.12)
project(loliLang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)
