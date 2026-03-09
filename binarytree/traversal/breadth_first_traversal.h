#pragma once

#include <queue>
#include "traversal.h"

/**
 * Class for performing breadth-first traversal on a binary tree.
 */
class BreadthFirstTraversal : public Traversal {
public:
    void traverse(TreeNode* root) const override {
        if (root == nullptr) return;
        std::queue<TreeNode*> queue;
        queue.push(root);
        while (!queue.empty()) {
            TreeNode* node = queue.front(); queue.pop();
            node->visit();
            if (node->get_left()  != nullptr) queue.push(node->get_left());
            if (node->get_right() != nullptr) queue.push(node->get_right());
        }
    }
};
