#ifndef TOKEN_H
#define TOKEN_H
#include <any>
#include <string>

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
    };
    static_assert (
            sizeof(FormaAsStr)/sizeof(FormaAsStr[0]) == (size_t)Forma::SIZE_OF_FORMA, 
            "new forma was introduced, but it haven't string value");

    class Token {
    private:
        std::string _lexeme;
        std::any    _literal; 
        Forma       _forma;
    
    public:
        Token();
        Token (Forma forma, const std::string& lexeme, std::any literal);


        std::string& lexeme  () const;
        std::any     literal () const;
        Forma        forma   () const;

        Token& lexeme  (const std::string& value);
        Token& literal (const std::any& value);
        Token& forma   (const Forma value);

        std::string asString() const;
    };
};
#endif
