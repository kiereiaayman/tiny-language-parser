#pragma once
#include "token.hpp"
#include <vector>

class TokenStream
{
public:
    TokenStream(const std::vector<Token> &tokens);

    const Token &peek(int offset = 0) const;
    const Token &advance();
    bool match(TokenType t);
    void expect(TokenType t);

private:
    std::vector<Token> tokens;
    size_t index;
};
