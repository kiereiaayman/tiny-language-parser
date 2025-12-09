#include <stdexcept>
#include <string>
#include "../include/tokenStream.hpp"
#include "../include/common.hpp"
#include "../include/scanner.hpp"

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
        string msg = "Parser error: expected ";
        msg += tokenToString(t);
        msg += " but found ";
        msg += tokenToString(peek().type);

        throw runtime_error(msg);
    }
}
