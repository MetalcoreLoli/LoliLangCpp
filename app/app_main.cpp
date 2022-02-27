//
// Created by danielf on 05.01.2022.
//

#include "loliLang/daphnie.h"
#include <cassert>
#include <cstddef>
#include <ostream>
#include <vector>
#define LOLI_LOG_STD
#include <iostream>
#include <fstream>
#include <loliLang/common.h>
#include <string>

#define ANSII_CLEAR_SCR "\x1b[27J"

struct STDOutWrite : public loli::IWriter {
    void Write (const std::string& message) override {
        std::cout << message << std::endl;
    }
};

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

int main (int argc, char** argv) {
    loli::LoliInter inter {};

    if (argc < 2) {
        std::cout << "the source code file was not pass throuht" << std::endl;
        return -1;
    }
    loli::ASTAsString ast{};
    
    std::ifstream sourceCode(argv[1]);
    std::string line;
    std::string code;
    std::vector<std::string> lines {};
    while (std::getline (sourceCode, line)) {
        if (!line.empty()) {
            if (line.substr(0, 4) == "    ") {
               code.append("t0R3pl4c3"); 
            }
            code.append(line).append("\n");
        }
    }

    auto c = replace_all(code, "\nt0R3pl4c3", "");
    line = "";
    for (auto& cc : code) {
        if (cc != '\n')  {
            line += cc;
        } else {
            lines.push_back(line);
            line = "";
        }
    }    
    {
#ifdef LOLI_LOG_STD
        for (auto& line : lines) {
            std::cout << line << std::endl;
        }
#endif
        for (auto& line : lines) {
            loli::Daphnie d {line};
#ifdef LOLI_LOG_STD
            std::cout << d.growTree()-> visit(&ast).Unwrap<std::string>() << std::endl;
#endif
            
            auto res = inter.AnswerOn(line);
            if (res.RawResult().TypeHashCode() == typeid(float).hash_code()) {
                std::cout << res.As<float>() << std::endl;
            }
        }
    }
}
