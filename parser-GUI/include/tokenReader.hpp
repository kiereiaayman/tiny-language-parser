#pragma once

#include <vector>
#include <fstream>
#include "token.hpp"
#include "scanner.hpp"

using namespace std;

vector<Token> readTokensFromFile(const string &filename);

