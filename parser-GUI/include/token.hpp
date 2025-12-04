#pragma once
#include "common.hpp"
#include "tokenType.hpp"

struct Token
{
    TokenType type;
    string value;
    int line;
};
