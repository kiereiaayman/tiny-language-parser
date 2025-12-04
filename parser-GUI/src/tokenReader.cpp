#include "tokenReader.hpp"
#include "common.hpp"

vector<Token> readTokensFromFile(const string &filename)
{
    ifstream source(filename);
    if (!source.is_open())
    {
        throw runtime_error("Cannot open file: " + filename);
    }

    vector<Token> tokens;
    string lexeme;
    int line = 1;

    while (true)
    {
        TokenType ttype = getToken(source, lexeme, line);

        Token t;
        t.type = ttype;
        t.value = lexeme;
        t.line = line;

        tokens.push_back(t);

        if (ttype == TokenType::ENDFILE)
            break;
    }

    return tokens;
}
