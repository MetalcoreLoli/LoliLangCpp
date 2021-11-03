#include <iostream>
#include "./lexer.h"

int main () {
    loli::Lexer lex;
/**
 *  func abs(a) {
 *     return if (a < 0) a * (-1) else a;  
 *  }
 *  func main () {
 *      a = 144561;
 *  }
 * */

    auto tokens = lex.lineToTokens (
            "func main() { foo = (2+5)/5*99 == 144561; >=<= <>; return if (2>1) {true} else {false}}");
    for (auto& token : tokens) {
        std::cout << token.asString() << std::endl;
    }
    return EXIT_SUCCESS;
}
