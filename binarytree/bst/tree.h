#pragma once

#include "../tree_node.h"

/**
 * Interface for a binary search tree.
 */
class Tree {
public:
    virtual ~Tree() = default;

    /**
     * Retrieves the root node of the tree.
     */
    virtual TreeNode* get_root() const = 0;

    /**
     * Inserts a new node with the specified key into the tree.
     */
    virtual void insert(int key) = 0;

    /**
     * Deletes the node with the specified key from the tree.
     * Note: renamed from 'delete' (C++ reserved keyword).
     */
    virtual void delete_key(int key) = 0;

    /**
     * Searches for a node by its key. Returns nullptr if not found.
     */
    virtual TreeNode* search(int key) const = 0;
};
