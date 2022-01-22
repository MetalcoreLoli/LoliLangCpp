//
// Created by danielf on 05.01.2022.
//

#include <iostream>
#include <loliLang/common.h>
#include <string>

int main () {
    loli::ASTAsString ast;
    loli::LoliInter inter {};

    auto a = inter.AnswerOn("add a b => a + b");
    auto helloStr = inter.AnswerOn("add 1 1").As<float>();

    std::cout << helloStr << std::endl;
}
