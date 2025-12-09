#pragma once
#include "tokenType.hpp"
#include "string"

using namespace std;

struct Token
{
    TokenType type;
    string value;
    int line;
};
