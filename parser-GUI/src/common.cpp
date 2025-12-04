#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cctype>
#include "common.hpp"

string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");

    if (start == string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

TokenType stringToTokenType(const std::string &s)
{
    static const std::unordered_map<std::string, TokenType> lut = {
        {"IF", TokenType::IF},
        {"THEN", TokenType::THEN},
        {"END", TokenType::END},
        {"ELSE", TokenType::ELSE},
        {"REPEAT", TokenType::REPEAT},
        {"UNTIL", TokenType::UNTIL},
        {"READ", TokenType::READ},
        {"WRITE", TokenType::WRITE},
        {"IDENTIFIER", TokenType::IDENTIFIER},
        {"NUMBER", TokenType::NUMBER},
        {"ASSIGN", TokenType::ASSIGN},
        {"LESSTHAN", TokenType::LESSTHAN},
        {"EQUAL", TokenType::EQUAL},
        {"PLUS", TokenType::PLUS},
        {"MINUS", TokenType::MINUS},
        {"MULT", TokenType::MULT},
        {"DIV", TokenType::DIV},
        {"OPENBRACKET", TokenType::OPENBRACKET},
        {"CLOSEDBRACKET", TokenType::CLOSEDBRACKET},
        {"SEMICOLON", TokenType::SEMICOLON},
        {"ERROR", TokenType::ERROR},
        {"ENDFILE", TokenType::ENDFILE}};

    auto it = lut.find(s);
    if (it == lut.end())
        throw std::runtime_error("Unknown token type: " + s);

    return it->second;
}
