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

    auto fibHelper  = inter.AnswerOn("fibHelper a b n => if (0 < n) fibHelper (a + b) a (n - 1) else a");
    auto fib        = inter.AnswerOn("fib n => fibHelper 0 1 n");
    auto helloStr = inter.AnswerOn("fib 10").As<float>();

    std::cout << helloStr << std::endl;
}
