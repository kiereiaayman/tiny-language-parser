#include "../include/TreeLayout.hpp"
#include <algorithm>

void TreeLayout::calculateLayout(ASTNode *root) {
  if (!root)
    return;

  // Pass 1: Calculate widths (bottom-up)
  calculateWidths(root);

  // Pass 2: Calculate positions (top-down)
  calculatePositions(root, 0, 0);
}

int TreeLayout::calculateWidths(ASTNode *node) {
  if (!node)
    return 0;

  // Calculate width of children block
  int childrenBlockWidth = 0;
  for (ASTNode *child : node->children) {
    // Recursively calculate width for child
    // This returns the total width of the child including its siblings
    childrenBlockWidth += calculateWidths(child) + H_SPACING;
  }
  if (!node->children.empty()) {
    childrenBlockWidth -= H_SPACING;
  }

  // Node's own subtree width (without its own siblings)
  // It is the max of the node's visual width and its children block
  node->width = std::max(NODE_WIDTH, childrenBlockWidth);

  // Calculate width of siblings (to return total width)
  int siblingsWidth = 0;
  for (ASTNode *sib : node->sibling) {
    siblingsWidth += calculateWidths(sib) + H_SPACING;
  }

  // Return total width: (Node Subtree) + (Siblings)
  return node->width + siblingsWidth;
}

void TreeLayout::calculatePositions(ASTNode *node, int x, int y) {
  if (!node)
    return;

  // 1. Position the Node itself
  // The node is centered within its subtree width (node->width)
  // node->x is the left edge of the visual rectangle
  // We want the rectangle (NODE_WIDTH) to be centered in node->width
  // So visualX = x + (node->width - NODE_WIDTH) / 2
  // But wait, 'x' passed in is the left edge of the allocated space.
  // So we store the visual coordinates in node->x, node->y?
  // Yes, for drawing.

  node->x = x + (node->width - NODE_WIDTH) / 2;
  node->y = y;

  // 2. Position Children
  // We need to calculate the total width of children again to center them
  int childrenTotalWidth = 0;
  for (ASTNode *child : node->children) {
    // We need the total width of child + its siblings
    // We can't just use child->width. We need to sum child and its siblings.
    // Helper to get total width
    int w = child->width;
    for (ASTNode *s : child->sibling)
      w += s->width + H_SPACING;

    childrenTotalWidth += w + H_SPACING;
  }
  if (!node->children.empty())
    childrenTotalWidth -= H_SPACING;

  // Start X for children (centered under parent)
  int currentChildX = x + (node->width - childrenTotalWidth) / 2;
  int childY = y + V_SPACING;

  for (ASTNode *child : node->children) {
    // Position this child (and its siblings recursively)
    calculatePositions(child, currentChildX, childY);

    // Advance X by child's total width
    int w = child->width;
    for (ASTNode *s : child->sibling)
      w += s->width + H_SPACING;
    currentChildX += w + H_SPACING;
  }

  // 3. Position Siblings
  // Siblings are placed to the right of this node's subtree
  int currentSibX = x + node->width + H_SPACING;

  for (ASTNode *sib : node->sibling) {
    calculatePositions(sib, currentSibX, y);

    // Advance X by sibling's total width (subtree + its siblings? usually none)
    int w = sib->width;
    for (ASTNode *s : sib->sibling)
      w += s->width + H_SPACING;
    currentSibX += w + H_SPACING;
  }
}