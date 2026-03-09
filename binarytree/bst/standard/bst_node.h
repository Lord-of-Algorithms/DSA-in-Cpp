#pragma once

#include <iostream>
#include <string>
#include "../../tree_node.h"

/**
 * Represents a node in a binary search tree, implementing the TreeNode interface.
 */
class BstNode : public TreeNode {
public:
    int   key;
    BstNode* left  = nullptr;
    BstNode* right = nullptr;

    explicit BstNode(int k) : key(k) {}

    int    get_key()   const override { return key; }
    TreeNode* get_left()  const override { return left; }
    TreeNode* get_right() const override { return right; }

    /**
     * Visits this node: prints the key to stdout.
     */
    void visit() const override { std::cout << key << " "; }

    std::string to_string() const override { return std::to_string(key); }
};
