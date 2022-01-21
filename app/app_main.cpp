//
// Created by danielf on 05.01.2022.
//

#include <iostream>
#include <loliLang/common.h>
#include <string>

int main () {
    loli::ASTAsString ast;
    loli::LoliInter inter {};
    auto helloStr = inter.AnswerOn("\"Hello, World\"").As<std::string>();

    std::cout << helloStr << std::endl;
}
