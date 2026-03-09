#pragma once

#include <stack>
#include "traversal.h"
#include "traversal_nature.h"

/**
 * Class for performing pre-order traversal on a binary tree.
 */
class PreOrderTraversal : public Traversal {
public:
    explicit PreOrderTraversal(TraversalNature nature) : nature_(nature) {}

    void traverse(TreeNode* root) const override {
        if (nature_ == TraversalNature::Iterative) {
            iterative_pre_order(root);
        } else {
            recursive_pre_order(root);
        }
    }

private:
    TraversalNature nature_;

    /**
     * Iteratively traverses a binary tree in pre-order manner.
     */
    static void iterative_pre_order(TreeNode* root) {
        if (root == nullptr) return;
        std::stack<TreeNode*> stack;
        stack.push(root);
        while (!stack.empty()) {
            TreeNode* node = stack.top(); stack.pop();
            node->visit();
            // Push right first so left is processed first
            if (node->get_right() != nullptr) stack.push(node->get_right());
            if (node->get_left()  != nullptr) stack.push(node->get_left());
        }
    }

    /**
     * Recursively traverses a binary tree in pre-order manner.
     */
    static void recursive_pre_order(TreeNode* node) {
        if (node == nullptr) return;
        node->visit();
        recursive_pre_order(node->get_left());
        recursive_pre_order(node->get_right());
    }
};
