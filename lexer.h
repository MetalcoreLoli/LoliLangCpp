#ifndef LEXER_H
#define LEXER_H
#include <cstddef>
#include <vector>
#include <algorithm>
#include <map>

#include "./token.h"
#include "./utils.h"

namespace loli {
    class Lexer {
    private:
        std::map<std::string, loli::Token> _symbolsTable {
            {"func", Token(Forma::FUNC, "func", 0)},
            {"if",   Token(Forma::IF,   "if", 0)},
            {"else", Token(Forma::ELSE, "else", 0)},
            {"true", Token(Forma::TRUE, "true", 0)},
            {"false", Token(Forma::FALSE, "false", 0)},
            {"return", Token(Forma::RETURN, "return", 0)},
            {"+", Token(Forma::ADD, "+", 0)}, 
            {"-", Token(Forma::SUB, "-", 0)},
            {"*", Token(Forma::MUL, "*", 0)}, 
            {"/", Token(Forma::DIV, "/", 0)}, 
            {"=", Token(Forma::DEFINE, "=", 0)}, 
            {"(", Token(Forma::LPAREN, "(", 0)}, 
            {")", Token(Forma::RPAREN, ")", 0)},
            {"{", Token(Forma::LCURL, "{", 0)}, 
            {"}", Token(Forma::RCURL, "}", 0)},
            {";", Token(Forma::SEMI, ";", 0)}, 
            {",", Token(Forma::COMMA, ",", 0)}, 
            {">", Token(Forma::GT, ">", 0)}, 
            {"<", Token(Forma::LT, "<", 0)}, 
            {"==", Token(Forma::EQ, "==", 0)}, 
            {">=", Token(Forma::GT_EQ, ">=", 0)}, 
            {"<=", Token(Forma::LT_EQ, "<=", 0)} 
        };

        size_t _current{};
        std::string _source;
        
        char Peek() const;
        char PeekNext() const;
        
        bool IsEnd() const;
        bool IsDigit(char symbol);
        bool IsKeyword(const std::string& value);

        utils::Maybe<std::string*> TryKeyword(); 

        std::string LexWord(const std::string& word, const std::string& context, size_t start);

        Lexer& MoveToNext();

        Token Match2(char current, char next);
        Token Number();
        Token Indentifier();
        Token Keyword(const std::string* word);
    public:
        Lexer();
        std::vector<Token> lineToTokens(const std::string& value);
    };
};
#endif
