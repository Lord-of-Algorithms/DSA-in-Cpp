#pragma once

#include "../tree_node.h"

/**
 * Interface for implementing different traversal strategies on a binary tree.
 */
class Traversal {
public:
    virtual ~Traversal() = default;

    /**
     * Performs traversal of a binary tree.
     *
     * @param root The root node of the tree to traverse.
     */
    virtual void traverse(TreeNode* root) const = 0;
};
