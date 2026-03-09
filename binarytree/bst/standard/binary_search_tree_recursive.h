#pragma once

#include "../tree.h"
#include "../bst_utils.h"
#include "bst_node.h"

/**
 * Implementation of a binary search tree with recursive methods.
 */
class BinarySearchTreeRecursive : public Tree {
public:
    ~BinarySearchTreeRecursive() override { delete_tree(root_); }

    TreeNode* get_root() const override { return root_; }

    void insert(int key) override {
        root_ = insert_recursive(root_, key);
    }

    void delete_key(int key) override {
        root_ = delete_recursive(root_, key);
    }

    TreeNode* search(int key) const override {
        return BstUtils::search_recursive(root_, key);
    }

private:
    BstNode* root_ = nullptr;

    /**
     * Recursively inserts a new key. Returns the (possibly new) subtree root.
     */
    static BstNode* insert_recursive(BstNode* current, int key) {
        if (current == nullptr) return new BstNode(key);

        if (key < current->key) {
            current->left = insert_recursive(current->left, key);
        } else if (key > current->key) {
            current->right = insert_recursive(current->right, key);
        } else {
            BstUtils::handle_duplicate_key(key);
        }
        return current;
    }

    /**
     * Recursively finds and deletes a node. Returns the (possibly new) subtree root.
     */
    static BstNode* delete_recursive(BstNode* current, int key) {
        if (current == nullptr) return nullptr;

        if (key < current->key) {
            current->left = delete_recursive(current->left, key);
        } else if (key > current->key) {
            current->right = delete_recursive(current->right, key);
        } else {
            // Case 1: Zero or one child
            if (current->left == nullptr) {
                BstNode* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                BstNode* temp = current->left;
                delete current;
                return temp;
            }
            // Case 2: Two children — replace with in-order successor's key
            current->key   = BstUtils::min_value(current->right);
            current->right = delete_recursive(current->right, current->key);
        }
        return current;
    }

    static void delete_tree(BstNode* node) {
        if (node) {
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }
    }
};
