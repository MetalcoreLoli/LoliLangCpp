//
// Created by danielf on 05.01.2022.
//

#include <iostream>
#include <loliLang/common.h>

int main () {
    loli::ASTAsString ast;
    auto helloStr =
            loli::unwrap<void, std::string>(
                    loli::Daphnie{"\"Hello, World\""}.growTree()->visit(&ast));

    std::cout << helloStr << std::endl;
}