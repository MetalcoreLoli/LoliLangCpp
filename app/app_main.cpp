//
// Created by danielf on 05.01.2022.
//

#define LOLI_LOG_STD
#include <iostream>
#include <loliLang/common.h>
#include <string>


int main () {
    loli::ASTAsString ast;
    loli::LoliInter inter {};

    auto a = inter.AnswerOn("fib a b n => if (0 < n) fib (a + b) a (1 - n) else a");
    auto helloStr = inter.AnswerOn("fib 0 1 10").As<float>();

    std::cout << helloStr << std::endl;
}
