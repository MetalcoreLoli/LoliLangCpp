#ifndef TOKEN_H
#define TOKEN_H
#include <algorithm>
#include <string>
#include <memory>
#include <vector>

namespace loli {
    enum class Forma {
       ADD =0,
       SUB,
       MUL,
       DIV,
       NUM,
       DEFINE,
       EQ,
       INDENTIFIER,
       FUNC,
       BANG_EQ,
       GT,
       LT,
       GT_EQ,
       LT_EQ,
       LPAREN,
       RPAREN,
       LCURL,
       RCURL,
       COMMA,
       SEMI,
       IF,
       ELSE,
       TRUE,
       FALSE,
       RETURN,
       IMPORT,
       LAMBDA_ARROW,
       STRING_LIT,
       DOT,
       CLASS,
       FOR,
       NIL,
       OR,
       WHILE,
       AND,
       FOR_,
       EOF_,
       WITH,
       END,
       WHERE,
       SIZE_OF_FORMA
    };
    inline Forma operator |(Forma a, Forma b) {
        return static_cast<loli::Forma>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline Forma operator &(Forma a, Forma b) {
        return static_cast<loli::Forma>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline Forma operator ^(Forma a, Forma b) {
        return static_cast<loli::Forma>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    static const char* FormaAsStr[] = {
       "ADD",
       "SUB",
       "MUL",
       "DIV",
       "NUM",
       "DEFINE",
       "EQ",
       "INDENTIFIER",
       "FUNC",
       "BANG_EQ",
       "GT",
       "LT",
       "GT_EQ",
       "LT_EQ",
       "LPAREN",
       "RPAREN",
       "LCURL",
       "RCURL",
       "COMMA",
       "SEMI",
       "IF",
       "ELSE",
       "TRUE",
       "FALSE",
       "RETURN",
       "IMPORT",
       "LAMBDA_ARROW",
       "STRING_LIT",
       "DOT",
       "CLASS",
       "FOR",
       "NIL",
       "OR",
       "WHILE",
       "AND",
       "FOR_",
       "EOF_",
       "WITH",
       "END",
       "WHERE",
    };
    static_assert (
            sizeof(FormaAsStr)/sizeof(FormaAsStr[0]) == (size_t)Forma::SIZE_OF_FORMA,
            "new forma was introduced, but it haven't string value");

    class Token {
    private:
        std::string             _lexeme;
        std::shared_ptr<void>   _literal;
        Forma                   _forma;
    
    public:
        Token();
        Token (Forma forma, const std::string& lexeme, std::shared_ptr<void> literal);


        [[nodiscard]] std::string& lexeme  () const;
        [[nodiscard]] std::shared_ptr<void> literal () const;
        [[nodiscard]] Forma        forma   () const;

        Token& lexeme  (const std::string& value);
        Token& literal (std::shared_ptr<void> value);
        Token& forma   (Forma value);

        [[nodiscard]] std::string asString() const;

        [[nodiscard]] static bool IsLiteral(const Token& token) {
           std::vector<Forma> literals = {
               Forma::NUM, Forma::STRING_LIT
           };
           return std::find(literals.begin(), literals.end(), token.forma()) != literals.end();
        }
    };
};
#endif
