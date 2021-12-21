add_library (
    loliLang 
    lib/loliLang/utils.h
    lib/loliLang/lexy.h
    lib/loliLang/lexy.cpp
    lib/loliLang/expression.h
    lib/loliLang/expression.cpp
    lib/loliLang/token.h
    lib/loliLang/token.cpp
    lib/loliLang/lexer.h
    lib/loliLang/lexer.cpp
    lib/loliLang/daphnie.h
    lib/loliLang/daphnie.cpp
    lib/loliLang/ast.cpp
    lib/loliLang/exceptions.h
    lib/loliLang/exceptions.cpp
    lib/loliLang/common.h
    )

target_include_directories(loliLang PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib")
