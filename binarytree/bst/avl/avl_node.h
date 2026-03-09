#pragma once

#include <iostream>
#include <string>
#include "../../tree_node.h"

/**
 * Represents a node in an AVL tree, implementing the TreeNode interface.
 */
class AvlNode : public TreeNode {
public:
    int      key;
    AvlNode* left   = nullptr;
    AvlNode* right  = nullptr;
    int      height = 0;  // Height of the node (leaf = 0)

    explicit AvlNode(int k) : key(k) {}

    int    get_key()   const override { return key; }
    TreeNode* get_left()  const override { return left; }
    TreeNode* get_right() const override { return right; }

    void visit() const override { std::cout << key << " "; }

    std::string to_string() const override { return std::to_string(key); }
};
