#pragma once
#include "ASTNodeType.hpp"
#include <string>
#include <vector>

class ASTNode
{
public:
    ASTNodeType type;
    std::string value;
    std::vector<ASTNode *> children;

    ASTNode(ASTNodeType t, const std::string &v = "")
        : type(t), value(v) {}
};
