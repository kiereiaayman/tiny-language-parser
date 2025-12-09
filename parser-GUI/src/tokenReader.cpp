#include "../include/tokenReader.hpp"
#include "../include/common.hpp"
#include <fstream>


vector<Token> readTokensFromFile(const string &filename)
{
    ifstream in(filename);
    if (!in.is_open())
    {
        throw runtime_error("Cannot open file: " + filename);
    }

    vector<Token> tokens;
    string line;
    int lineNumber = 1;

    while (getline(in, line))
    {
        if (line.empty())
            continue;

        size_t commaPos = line.find(',');
        if (commaPos == string::npos)
            throw runtime_error("Invalid token format at line " + to_string(lineNumber));

        string lexeme = (line.substr(0, commaPos));
        lexeme = trim(lexeme);
        string typeStr = (line.substr(commaPos + 1));
        typeStr = trim(typeStr);

        Token t;
        t.value = lexeme;
        t.type = stringToTokenType(typeStr);
        t.line = lineNumber;

        tokens.push_back(t);
        lineNumber++;
    }

    return tokens;
}
