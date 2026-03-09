#pragma once

#include <algorithm>
#include "../tree.h"
#include "../bst_utils.h"
#include "avl_node.h"

/**
 * Represents an AVL tree — a self-balancing binary search tree.
 * After every insert or delete the tree rebalances via rotations.
 */
class AvlTree : public Tree {
public:
    ~AvlTree() override { delete_tree(root_); }

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
    AvlNode* root_ = nullptr;

    // ─── Height helpers ──────────────────────────────────────────────────────

    static int get_height(AvlNode* node) {
        return node ? node->height : -1;  // null node has height -1
    }

    static void update_height(AvlNode* node) {
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    }

    static int get_balance(AvlNode* node) {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    // ─── Rotations ───────────────────────────────────────────────────────────

    static AvlNode* right_rotate(AvlNode* rotated) {
        AvlNode* new_root      = rotated->left;
        AvlNode* re_parented   = new_root->right;
        new_root->right        = rotated;
        rotated->left          = re_parented;
        update_height(rotated);
        update_height(new_root);
        return new_root;
    }

    static AvlNode* left_rotate(AvlNode* rotated) {
        AvlNode* new_root      = rotated->right;
        AvlNode* re_parented   = new_root->left;
        new_root->left         = rotated;
        rotated->right         = re_parented;
        update_height(rotated);
        update_height(new_root);
        return new_root;
    }

    // ─── Insert ──────────────────────────────────────────────────────────────

    static AvlNode* insert_recursive(AvlNode* current, int key) {
        if (current == nullptr) return new AvlNode(key);

        if (key < current->key) {
            current->left  = insert_recursive(current->left, key);
        } else if (key > current->key) {
            current->right = insert_recursive(current->right, key);
        } else {
            BstUtils::handle_duplicate_key(key);
            return current;
        }

        update_height(current);
        int balance = get_balance(current);

        // Single Right Rotation (LL)
        if (balance > 1 && key < current->left->key)
            return right_rotate(current);

        // Single Left Rotation (RR)
        if (balance < -1 && key > current->right->key)
            return left_rotate(current);

        // Left-Right Rotation (LR)
        if (balance > 1 && key > current->left->key) {
            current->left = left_rotate(current->left);
            return right_rotate(current);
        }

        // Right-Left Rotation (RL)
        if (balance < -1 && key < current->right->key) {
            current->right = right_rotate(current->right);
            return left_rotate(current);
        }

        return current;
    }

    // ─── Delete ──────────────────────────────────────────────────────────────

    static AvlNode* delete_recursive(AvlNode* current, int key) {
        if (current == nullptr) return nullptr;

        if (key < current->key) {
            current->left  = delete_recursive(current->left, key);
        } else if (key > current->key) {
            current->right = delete_recursive(current->right, key);
        } else {
            // Case 1: Zero or one child
            if (current->left == nullptr) {
                AvlNode* temp = current->right;
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                AvlNode* temp = current->left;
                delete current;
                return temp;
            }
            // Case 2: Two children — replace with in-order successor
            current->key   = BstUtils::min_value(current->right);
            current->right = delete_recursive(current->right, current->key);
        }

        update_height(current);
        int balance = get_balance(current);

        // Single Right Rotation
        if (balance > 1 && get_balance(current->left) >= 0)
            return right_rotate(current);

        // Left-Right Rotation
        if (balance > 1 && get_balance(current->left) < 0) {
            current->left = left_rotate(current->left);
            return right_rotate(current);
        }

        // Single Left Rotation
        if (balance < -1 && get_balance(current->right) <= 0)
            return left_rotate(current);

        // Right-Left Rotation
        if (balance < -1 && get_balance(current->right) > 0) {
            current->right = right_rotate(current->right);
            return left_rotate(current);
        }

        return current;
    }

    static void delete_tree(AvlNode* node) {
        if (node) {
            delete_tree(node->left);
            delete_tree(node->right);
            delete node;
        }
    }
};
