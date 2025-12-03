#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "common.hpp"
#include "tokenType.hpp"

// Function declarations
string tokenToString(TokenType t);
TokenType getToken(ifstream &source, string &tokenString, int &lineNumber);

#endif // SCANNER_HPP
