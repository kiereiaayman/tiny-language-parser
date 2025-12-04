#include "tokenStream.hpp"
#include "common.hpp"

TokenStream::TokenStream(const vector<Token> &tokens)
    : tokens(tokens), index(0) {}

const Token &TokenStream::peek(int offset) const
{
    size_t pos = index + offset;

    if (pos >= tokens.size())
        return tokens.back();   

    return tokens[pos];
}

const Token &TokenStream::advance()
{
    if (index < tokens.size())
        index++;

    return tokens[index - 1];
}

bool TokenStream::match(TokenType t)
{
    if (peek().type == t)
    {
        advance();
        return true;
    }
    return false;
}

void TokenStream::expect(TokenType t)
{
    if (!match(t))
    {
        string msg = "Parser error: expected token type ";
        msg += to_string(t);
        msg += " but found ";
        msg += to_string(peek().type);

        throw runtime_error(msg);
    }
}
