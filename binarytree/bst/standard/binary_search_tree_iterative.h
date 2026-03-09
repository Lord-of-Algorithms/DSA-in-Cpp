#pragma once

#include <iostream>
#include "../tree.h"
#include "../bst_utils.h"
#include "bst_node.h"

/**
 * Implementation of a binary search tree with iterative methods.
 */
class BinarySearchTreeIterative : public Tree {
public:
    ~BinarySearchTreeIterative() override { delete_tree(root_); }

    TreeNode* get_root() const override { return root_; }

    void insert(int key) override {
        BstNode* new_node = new BstNode(key);
        if (root_ == nullptr) {
            root_ = new_node;
            return;
        }

        BstNode* current = root_;
        BstNode* parent  = nullptr;

        // Traverse to find the appropriate insertion point
        while (current != nullptr) {
            parent = current;
            if (key < current->key) {
                current = current->left;
            } else if (key > current->key) {
                current = current->right;
            } else {
                BstUtils::handle_duplicate_key(key);
                delete new_node;
                return;
            }
        }

        // Attach the new node to its parent
        if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }

    void delete_key(int key) override {
        if (root_ == nullptr) {
            std::cout << "The tree is empty.\n";
            return;
        }

        BstNode* node_to_delete = root_;
        BstNode* parent_node    = nullptr;

        // Find the node to delete and its parent
        while (node_to_delete != nullptr && node_to_delete->key != key) {
            parent_node = node_to_delete;
            node_to_delete = key < node_to_delete->key
                ? node_to_delete->left : node_to_delete->right;
        }

        if (node_to_delete == nullptr) {
            std::cout << "BstNode with key " << key << " not found.\n";
            return;
        }

        bool is_root_node  = (node_to_delete == root_);
        bool is_left_child = (parent_node != nullptr && node_to_delete->key < parent_node->key);

        // Case 1: Leaf node (no children)
        if (node_to_delete->left == nullptr && node_to_delete->right == nullptr) {
            if (is_root_node)       root_             = nullptr;
            else if (is_left_child) parent_node->left = nullptr;
            else                    parent_node->right = nullptr;
            delete node_to_delete;
        }
        // Case 2: One child
        else if (node_to_delete->left == nullptr || node_to_delete->right == nullptr) {
            BstNode* child = node_to_delete->left ? node_to_delete->left : node_to_delete->right;
            if (is_root_node)       root_             = child;
            else if (is_left_child) parent_node->left = child;
            else                    parent_node->right = child;
            delete node_to_delete;
        }
        // Case 3: Two children — find in-order successor
        else {
            BstNode* successor        = node_to_delete->right;
            BstNode* successor_parent = node_to_delete;
            while (successor->left != nullptr) {
                successor_parent = successor;
                successor        = successor->left;
            }

            if (successor != node_to_delete->right) {
                successor_parent->left = successor->right;
                successor->right       = node_to_delete->right;
            }
            successor->left = node_to_delete->left;

            if (is_root_node)       root_             = successor;
            else if (is_left_child) parent_node->left = successor;
            else                    parent_node->right = successor;
            delete node_to_delete;
        }
    }

    TreeNode* search(int key) const override {
        BstNode* current = root_;
        while (current != nullptr) {
            if (key == current->key) return current;
            current = key < current->key ? current->left : current->right;
        }
        return nullptr;
    }

private:
    BstNode* root_ = nullptr;

    static void delete_tree(BstNode* node) {
        if (node) {
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }
    }
};
