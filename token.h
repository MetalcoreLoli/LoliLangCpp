#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <memory>

namespace loli {
    enum class Forma {
       ADD = 0,
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
       SIZE_OF_FORMA
    };

    static const char* FormaAsStr[] = {
       "ADD",
       "SUB",
       "MUL",
       "DIV",
       "NUM",
       "EQ",
       "DEFINE",
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


        std::string& lexeme  () const;
        std::shared_ptr<void> literal () const;
        Forma        forma   () const;

        Token& lexeme  (const std::string& value);
        Token& literal (const std::shared_ptr<void> value);
        Token& forma   (const Forma value);

        std::string asString() const;
    };
};
#endif
