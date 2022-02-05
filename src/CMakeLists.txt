add_library (
    loliLang

    lib/loliLang/types.h
    lib/loliLang/types.cpp
    lib/loliLang/call.h
    lib/loliLang/call.cpp
    lib/loliLang/expressionFactory.hpp
    lib/loliLang/expressionConverter.h
    lib/loliLang/expressionConverter.cpp
    lib/loliLang/memory.h
    lib/loliLang/memory.cpp
    lib/loliLang/defCaller.h
    lib/loliLang/defCaller.cpp
    lib/loliLang/loliInter.h
    lib/loliLang/loliInter.cpp
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
    lib/loliLang/gramm.h
    lib/loliLang/gramm.cpp
    lib/loliLang/common.h
    )

target_include_directories(loliLang PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib")
