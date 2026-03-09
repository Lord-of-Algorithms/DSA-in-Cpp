#pragma once

#include <stack>
#include "traversal.h"
#include "traversal_nature.h"

/**
 * Class for performing post-order traversal on a binary tree.
 */
class PostOrderTraversal : public Traversal {
public:
    explicit PostOrderTraversal(TraversalNature nature) : nature_(nature) {}

    void traverse(TreeNode* root) const override {
        if (nature_ == TraversalNature::Iterative) {
            iterative_post_order(root);
        } else {
            recursive_post_order(root);
        }
    }

private:
    TraversalNature nature_;

    /**
     * Iteratively traverses a binary tree in post-order manner using two stacks.
     */
    static void iterative_post_order(TreeNode* root) {
        if (root == nullptr) return;
        std::stack<TreeNode*> stack1, stack2;
        stack1.push(root);
        while (!stack1.empty()) {
            TreeNode* node = stack1.top(); stack1.pop();
            stack2.push(node);
            if (node->get_left()  != nullptr) stack1.push(node->get_left());
            if (node->get_right() != nullptr) stack1.push(node->get_right());
        }
        while (!stack2.empty()) {
            stack2.top()->visit();
            stack2.pop();
        }
    }

    /**
     * Recursively traverses a binary tree in post-order manner.
     */
    static void recursive_post_order(TreeNode* root) {
        if (root == nullptr) return;
        recursive_post_order(root->get_left());
        recursive_post_order(root->get_right());
        root->visit();
    }
};
