#ifndef LEXER_H
#define LEXER_H
#include <cstddef>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

#include "./token.h"
#include "./utils.h"

namespace loli {
    class Lexer {
    private:
        size_t _currentLine;

        std::map<std::string, loli::Token> _symbolsTable {
            {"func", Token(Forma::FUNC, "func", 0)},
            {"if",   Token(Forma::IF,   "if", 0)},
            {"else", Token(Forma::ELSE, "else", 0)},
            {"true", Token(Forma::TRUE, "true", utils::newLink<bool>(true))},
            {"false", Token(Forma::FALSE, "false", utils::newLink<bool>(false))},
            {"return", Token(Forma::RETURN, "return", 0)},
            {"import", Token(Forma::IMPORT, "import", 0)},
            {"class", Token(Forma::CLASS, "class", 0)},
            {"nil", Token(Forma::NIL, "nil",0)},
            {"and",Token(Forma::AND, "and", 0)},
            {"or",Token(Forma::OR, "or", 0)},
            {"while",Token(Forma::WHILE, "while", 0)},
            {"for",Token(Forma::FOR, "for", 0)},
            {"with",Token(Forma::WITH, "with", 0)},
            {"end",Token(Forma::END, "end", 0)},
            {".",Token(Forma::DOT, ".", 0)},
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
            {"=>", Token(Forma::LAMBDA_ARROW, "=>", 0)},
            {"<=", Token(Forma::LT_EQ, "<=", 0)} 
        };

        size_t _current{};
        std::string _source;
        
        [[nodiscard]] char Peek() const;
        [[nodiscard]] char PeekNext() const;
        
        [[nodiscard]] bool IsEnd() const;
        bool IsDigit(char symbol);
        bool IsKeyword(const std::string&);

        utils::Maybe<std::string*> TryKeyword(); 

        std::string LexWord(
                const std::string& word, const std::string& context, size_t start);

        Lexer& MoveToNext();

        Token Match2(char current, char next);
        Token Match2OneOf(char current, char a, char b);
        Token StringLit();
        Token Number();
        Token Indentifier();
        Token Keyword(const std::string* word);
    public:
        Lexer();
        std::vector<Token> lineToTokens(std::string_view value);

        static std::vector<Token> Translate(std::string_view code);
    };
};
#endif
