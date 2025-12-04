#pragma once

#include <vector>
#include <fstream>
#include "token.hpp"
#include "common.hpp"

using namespace std;

vector<Token> readTokensFromFile(const string &filename);
