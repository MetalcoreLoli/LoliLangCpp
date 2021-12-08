#include "./token.h"


loli::Token& loli::Token::lexeme(const std::string& value) {
    _lexeme = value;
    return *this;
}

loli::Token& loli::Token::literal(const std::shared_ptr<void> value) {
    _literal = value;
    return *this;
}

loli::Token& loli::Token::forma(loli::Forma value) {
    _forma = value;
    return *this;
}
std::string& loli::Token::lexeme  () const {
    return const_cast<std::string&>(_lexeme);
}

std::shared_ptr<void> loli::Token::literal () const {
    return _literal;
}

loli::Forma  loli::Token::forma   () const {
    return _forma;
}

loli::Token::Token() {}
loli::Token::Token(loli::Forma forma, const std::string& lexeme, std::shared_ptr<void> literal) {
    _forma = forma;
    _lexeme = lexeme;
    _literal = literal;
}
std::string loli::Token::asString() const {
    std::string res = "[";
    res
        .append(loli::FormaAsStr[(size_t)_forma])
        .append(";\"")
        .append(_lexeme)
        .append("\"")
        .append("]");
    return res;
}
