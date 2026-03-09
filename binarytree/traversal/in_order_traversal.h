#pragma once

#include <stack>
#include "traversal.h"
#include "traversal_nature.h"

/**
 * Class for performing in-order traversal on a binary tree.
 */
class InOrderTraversal : public Traversal {
public:
    explicit InOrderTraversal(TraversalNature nature) : nature_(nature) {}

    void traverse(TreeNode* root) const override {
        if (nature_ == TraversalNature::Iterative) {
            iterative_in_order(root);
        } else {
            recursive_in_order(root);
        }
    }

private:
    TraversalNature nature_;

    /**
     * Iteratively traverses a binary tree in in-order manner.
     */
    static void iterative_in_order(TreeNode* root) {
        if (root == nullptr) return;
        std::stack<TreeNode*> stack;
        TreeNode* node = root;
        while (!stack.empty() || node != nullptr) {
            if (node != nullptr) {
                stack.push(node);
                node = node->get_left();
            } else {
                node = stack.top(); stack.pop();
                node->visit();
                node = node->get_right();
            }
        }
    }

    /**
     * Recursively traverses a binary tree in in-order manner.
     */
    static void recursive_in_order(TreeNode* node) {
        if (node == nullptr) return;
        recursive_in_order(node->get_left());
        node->visit();
        recursive_in_order(node->get_right());
    }
};
