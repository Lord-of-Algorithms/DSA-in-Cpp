#pragma once

#include <string>

/**
 * Interface defining the basic structure and functionality of a node in a binary tree.
 */
class TreeNode {
public:
    virtual ~TreeNode() = default;

    /**
     * Gets the key value of this node.
     */
    virtual int get_key() const = 0;

    /**
     * Gets the left child of this node, or nullptr if none.
     */
    virtual TreeNode* get_left() const = 0;

    /**
     * Gets the right child of this node, or nullptr if none.
     */
    virtual TreeNode* get_right() const = 0;

    /**
     * Processes or visits the current node (prints the key).
     */
    virtual void visit() const = 0;

    /**
     * Returns a string representation of the node (used by the tree printer).
     */
    virtual std::string to_string() const = 0;
};
