#include <iostream>
#include "./lexer.h"

int main () {
    loli::Lexer lex;
    std::string code = 
        "import 'io' \n"
        "func main => \n"
        "     println \"Hello, world\";";
    auto tokens = lex.lineToTokens (code);
    for (const auto& token : tokens) {
        std::cout << token.asString() << std::endl;
    }
    return EXIT_SUCCESS;
}
