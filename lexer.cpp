#include "./lexer.h"
#include <algorithm>
#include <stdexcept>

loli::Lexer::Lexer() {
    _current = 0;
}

char loli::Lexer::Peek() const {
    return _source[_current];    
}
char loli::Lexer::PeekNext() const {
    return _source[_current + 1];    
}
bool loli::Lexer::IsEnd() const {
    if (_current >= _source.length()) return true;
    return false;    
}
loli::Lexer& loli::Lexer::MoveToNext() {
    if (!IsEnd()) _current++;
    return *this;
}

bool loli::Lexer::IsDigit(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool loli::Lexer::IsKeyword(const std::string& value) {
    return _symbolsTable.contains(value);
}

std::string loli::Lexer::LexWord (
        const std::string& word, 
        const std::string& context,
        size_t start) {
    std::string head= "";
    char current = Peek();
    for (auto symbol : word) {
        if (symbol != current) {
            break;
        } 
        head += current;
        current = MoveToNext().Peek();
    }
    return head;
}

loli::utils::Maybe<std::string*> loli::Lexer::TryKeyword() {
    size_t start = _current;
    std::string context = _source.substr(start);
    for (const auto& [word, _]: _symbolsTable) {
        if (LexWord(word, context, _current) == word) {
            return utils::Maybe<std::string*>(new std::string(word)); 
        } 
        _current = start;
    }
    return utils::Maybe<std::string*>(nullptr);
}

loli::Token loli::Lexer::Keyword(const std::string* word) {
    std::string keyword = *word;
    if (IsKeyword(keyword)) {
        return _symbolsTable[keyword];
    } else {
         std::string msg = "unknown keyword \""+ keyword+ "\"";
         throw std::runtime_error {msg};
    }
}

loli::Token loli::Lexer::Number() {
    char current = Peek();
    std::string tail = "";
    while (IsDigit(current) && !IsEnd()) {
        tail += current;
        current = MoveToNext().Peek();
    }
    _current--;
    return Token(loli::Forma::NUM, tail, std::stoi(tail));
}

loli::Token loli::Lexer::Indentifier() {
    char current = Peek();    
    std::string identi = "";
    while (!IsEnd() && !IsKeyword(std::string(1, current)) && current != ' ') {
        if (IsKeyword(identi)) break;
        identi += current;
        current = MoveToNext().Peek();
    }
    _current --;
    return Token(Forma::INDENTIFIER, identi, 0);
}

loli::Token loli::Lexer::Match2(char current, char next) {
    std::string key = std::string(1, current);
    if (PeekNext() == next && !IsEnd()) {
        key += MoveToNext().Peek();
    } 
    return _symbolsTable[key];
}

loli::Token loli::Lexer::Match2OneOf(char current, char a, char b) {
    std::string key = std::string(1, current);
    if (PeekNext() == b && !IsEnd()) {
        key += MoveToNext().Peek();
    } else if (PeekNext() == a && !IsEnd()) {
        key += MoveToNext().Peek();
    } 
    return _symbolsTable[key];
}

loli::Token loli::Lexer::StringLit() {
    char current = MoveToNext().Peek();
    std::string val = "";

    while (current != '"' && !IsEnd()) {
        val += current;
        current = MoveToNext().Peek();
    }
    return {Forma::STRING_LIT, val, 0};
}

std::vector<loli::Token> loli::Lexer::lineToTokens(const std::string& value) {
    std::vector<loli::Token> res{};
    _source = value;

    char current = Peek();
    while (!IsEnd()) {
        switch (current) {
            case '\n': break;
            case '\t': break;
            case ' ': break;
            case '+': {
                    res.push_back(Token(loli::Forma::ADD, "+", 0));
            } break;
            case '-': {
                    res.push_back(Token(loli::Forma::SUB, "-", 0));
            } break;
            case '*': {
                    res.push_back(Token(loli::Forma::MUL, "*", 0));
            } break;
            case '/': {
                    res.push_back(Token(loli::Forma::DIV, "/", 0));
            } break;
            case '>': { 
                    res.push_back(Match2(current, '='));
            } break;
            case '<': {
                    res.push_back(Match2(current, '='));
            } break;
            case '=':  {
                    res.push_back(Match2OneOf(current, '=', '>'));
            } break;
            case '(': {
                res.push_back(Token(Forma::LPAREN, "(", 0));
            } break;
            case ')': {
                res.push_back(Token(Forma::RPAREN, ")", 0));
            } break;
            case '{': {
                res.push_back(Token(Forma::LCURL, "{", 0));
            } break;
            case '}': {
                res.push_back(Token(Forma::RCURL, "}", 0));
            } break;
            case ',': {
                res.push_back(Token(Forma::COMMA, ",", 0));
            } break;
            case ';': {
                res.push_back(Token(Forma::SEMI, ";", 0));
            } break;
            case '"': {
                res.push_back(StringLit());
            } break;
            default: 
                if (IsDigit(current)) {
                    res.push_back(Number());
                } else {
                    auto keyword = TryKeyword();
                    if (!keyword.HasValue())
                        res.push_back(Indentifier());
                    else 
                        res.push_back(Keyword(keyword.Value()));
                }
                break;
        } 
        current = MoveToNext().Peek();
    }
    return res;
}
