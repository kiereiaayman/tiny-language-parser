#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cctype>
using namespace std;

enum class TokenType
{
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    LESSTHAN,
    EQUAL,
    PLUS,
    MINUS,
    MULT,
    DIV,
    OPENBRACKET,
    CLOSEDBRACKET,
    SEMICOLON,
    ERROR,
    ENDFILE
};

// Function declarations
string tokenToString(TokenType t);
TokenType getToken(ifstream &source, string &tokenString, int &lineNumber);

#endif // SCANNER_HPP
