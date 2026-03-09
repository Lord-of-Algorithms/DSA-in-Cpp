#pragma once

#include <iostream>
#include "../tree_node.h"

/**
 * Helper class containing common methods for binary search trees.
 */
class BstUtils {
public:
    BstUtils() = delete;  // Prevent instantiation

    /**
     * Gets the in-order successor's key (smallest value in the subtree).
     */
    static int min_value(TreeNode* root) {
        while (root->get_left() != nullptr) root = root->get_left();
        return root->get_key();
    }

    /**
     * Recursively searches for a node with the given key.
     *
     * @return The found node, or nullptr if not found.
     */
    static TreeNode* search_recursive(TreeNode* current, int key) {
        if (current == nullptr) return nullptr;
        if (key == current->get_key()) return current;
        return key < current->get_key()
            ? search_recursive(current->get_left(), key)
            : search_recursive(current->get_right(), key);
    }

    /**
     * Handles the scenario where a duplicate key is encountered during insertion.
     */
    static void handle_duplicate_key(int key) {
        std::cout << "Attempted to insert duplicate key: " << key << "\n";
    }
};
