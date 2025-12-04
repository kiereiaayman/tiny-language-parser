#pragma once
#include "ASTNodeType.hpp"
#include <string>
#include <vector>

class ASTNode {
public:
  ASTNodeType type;
  std::string value;
  std::vector<ASTNode *> children;
  std::vector<ASTNode *> sibling;

  ASTNode(ASTNodeType t, const std::string &v = "") : type(t), value(v) {}

  // Layout properties
  int x = 0;
  int y = 0;
  int width = 0;
};
