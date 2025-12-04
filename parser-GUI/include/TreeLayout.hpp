#pragma once
#include "ASTNode.hpp"

class TreeLayout {
public:
  static void calculateLayout(ASTNode *root);

private:
  static int calculateWidths(ASTNode *node);
  static void calculatePositions(ASTNode *node, int x, int y);

  static constexpr int NODE_WIDTH = 100;
  static constexpr int NODE_HEIGHT = 50;
  static constexpr int H_SPACING = 20;
  static constexpr int V_SPACING = 80;
};
