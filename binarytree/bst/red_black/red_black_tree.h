#pragma once

#include <iostream>
#include "../tree.h"
#include "../bst_utils.h"
#include "rb_node.h"

/**
 * Represents a Red-Black Tree — a self-balancing binary search tree.
 * Each node is colored red or black; balancing properties ensure O(log n) operations.
 */
class RedBlackTree : public Tree {
public:
    RedBlackTree() {
        nil_  = RbNode::create_sentinel_node();
        root_ = nil_;
    }

    ~RedBlackTree() override {
        delete_rb_tree(root_);
        delete nil_;  // Delete the shared sentinel last
    }

    TreeNode* get_root() const override {
        return (root_ != nil_) ? root_ : nullptr;
    }

    // ─── Insert ──────────────────────────────────────────────────────────────

    void insert(int key) override {
        RbNode* new_node = new RbNode(key, nil_);
        if (root_ == nil_) {
            root_ = new_node;
            root_->set_black();  // Root is always black
            return;
        }

        RbNode* current = root_;
        RbNode* parent  = nil_;

        while (current != nil_) {
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

        new_node->parent = parent;
        if (key < parent->key) parent->left  = new_node;
        else                   parent->right = new_node;

        fix_insert(new_node);
    }

    // ─── Delete ──────────────────────────────────────────────────────────────

    void delete_key(int key) override {
        if (root_ == nil_) {
            std::cout << "The tree is empty.\n";
            return;
        }

        RbNode* node_to_delete = root_;
        while (node_to_delete != nil_ && node_to_delete->key != key) {
            node_to_delete = key < node_to_delete->key
                ? node_to_delete->left : node_to_delete->right;
        }

        if (node_to_delete == nil_) {
            std::cout << "Node with key " << key << " not found.\n";
            return;
        }

        // Zero or one child
        if (node_to_delete->left == nil_ || node_to_delete->right == nil_) {
            bool      deleting_black = node_to_delete->is_black();
            RbNode*   replacer       = nil_;

            if (node_to_delete->left == nil_) {
                replacer = node_to_delete->right;
                replace_subtree(node_to_delete, replacer);
            } else {
                replacer = node_to_delete->left;
                replace_subtree(node_to_delete, replacer);
            }
            delete node_to_delete;
            if (deleting_black) fix_delete(replacer);
        }
        // Two children
        else {
            RbNode* successor = node_to_delete->right;
            while (successor->left != nil_) successor = successor->left;

            bool    successor_was_black = successor->is_black();
            RbNode* successor_child     = successor->right;

            if (successor->parent != node_to_delete) {
                replace_subtree(successor, successor->right);
                successor->right = node_to_delete->right;
            }
            successor->right->parent = successor;

            replace_subtree(node_to_delete, successor);
            successor->left         = node_to_delete->left;
            successor->left->parent = successor;

            transfer_color(node_to_delete, successor);
            delete node_to_delete;

            if (successor_was_black) fix_delete(successor_child);
        }
    }

    // ─── Search ──────────────────────────────────────────────────────────────

    TreeNode* search(int key) const override {
        return BstUtils::search_recursive(get_root(), key);
    }

private:
    RbNode* root_;
    RbNode* nil_;   // Sentinel node representing NIL

    // ─── Rotations ───────────────────────────────────────────────────────────

    void rotate_left(RbNode* rotated) {
        RbNode* new_root    = rotated->right;
        RbNode* re_parented = new_root->left;
        rotated->right      = re_parented;
        if (re_parented != nil_) re_parented->parent = rotated;
        new_root->parent = rotated->parent;
        if      (rotated->parent == nil_)              root_ = new_root;
        else if (rotated == rotated->parent->left)     rotated->parent->left  = new_root;
        else                                           rotated->parent->right = new_root;
        new_root->left   = rotated;
        rotated->parent  = new_root;
    }

    void rotate_right(RbNode* rotated) {
        RbNode* new_root    = rotated->left;
        RbNode* re_parented = new_root->right;
        rotated->left       = re_parented;
        if (re_parented != nil_) re_parented->parent = rotated;
        new_root->parent = rotated->parent;
        if      (rotated->parent == nil_)              root_ = new_root;
        else if (rotated == rotated->parent->right)    rotated->parent->right = new_root;
        else                                           rotated->parent->left  = new_root;
        new_root->right  = rotated;
        rotated->parent  = new_root;
    }

    // ─── Fix after insert ────────────────────────────────────────────────────

    void fix_insert(RbNode* current) {
        RbNode* parent = current->parent;
        while (parent->is_red()) {
            RbNode* uncle;
            RbNode* grand_parent = parent->parent;
            if (parent == grand_parent->right) {
                uncle = grand_parent->left;
                if (uncle->is_red()) {  // Case 1: Uncle is red
                    uncle->set_black();
                    parent->set_black();
                    grand_parent->set_red();
                    current = grand_parent;
                    parent  = current->parent;
                } else {
                    if (current == parent->left) {  // Case 2
                        current = parent;
                        rotate_right(current);
                        parent       = current->parent;
                        grand_parent = parent->parent;
                    }
                    parent->set_black();      // Case 3
                    grand_parent->set_red();
                    rotate_left(grand_parent);
                }
            } else {
                uncle = grand_parent->right;
                if (uncle->is_red()) {  // Case 1: Uncle is red
                    uncle->set_black();
                    parent->set_black();
                    grand_parent->set_red();
                    current = grand_parent;
                    parent  = current->parent;
                } else {
                    if (current == parent->right) {  // Case 2
                        current = parent;
                        rotate_left(current);
                        parent       = current->parent;
                        grand_parent = parent->parent;
                    }
                    parent->set_black();      // Case 3
                    grand_parent->set_red();
                    rotate_right(grand_parent);
                }
            }
            if (current == root_) break;
        }
        root_->set_black();
    }

    // ─── Fix after delete ────────────────────────────────────────────────────

    void fix_delete(RbNode* current) {
        while (current != root_ && current->is_black()) {
            RbNode* sibling;
            if (current == current->parent->left) {
                sibling = current->parent->right;
                if (sibling->is_red()) {  // Case 1
                    sibling->set_black();
                    current->parent->set_red();
                    rotate_left(current->parent);
                    sibling = current->parent->right;
                }
                if (sibling->left->is_black() && sibling->right->is_black()) {  // Case 2
                    sibling->set_red();
                    current = current->parent;
                } else {
                    if (sibling->right->is_black()) {  // Case 3
                        sibling->left->set_black();
                        sibling->set_red();
                        rotate_right(sibling);
                        sibling = current->parent->right;
                    }
                    transfer_color(current->parent, sibling);  // Case 4
                    current->parent->set_black();
                    sibling->right->set_black();
                    rotate_left(current->parent);
                    current = root_;
                }
            } else {
                sibling = current->parent->left;
                if (sibling->is_red()) {  // Case 1
                    sibling->set_black();
                    current->parent->set_red();
                    rotate_right(current->parent);
                    sibling = current->parent->left;
                }
                if (sibling->right->is_black() && sibling->left->is_black()) {  // Case 2
                    sibling->set_red();
                    current = current->parent;
                } else {
                    if (sibling->left->is_black()) {  // Case 3
                        sibling->right->set_black();
                        sibling->set_red();
                        rotate_left(sibling);
                        sibling = current->parent->left;
                    }
                    transfer_color(current->parent, sibling);  // Case 4
                    current->parent->set_black();
                    sibling->left->set_black();
                    rotate_right(current->parent);
                    current = root_;
                }
            }
        }
        current->set_black();
    }

    // ─── Helpers ─────────────────────────────────────────────────────────────

    void replace_subtree(RbNode* target, RbNode* replacer) {
        if      (target->parent == nil_)           root_                  = replacer;
        else if (target == target->parent->left)   target->parent->left   = replacer;
        else                                       target->parent->right  = replacer;
        replacer->parent = target->parent;
    }

    static void transfer_color(RbNode* source, RbNode* target) {
        if (source->is_red()) target->set_red();
        else                  target->set_black();
    }

    void delete_rb_tree(RbNode* node) {
        if (node && node != nil_) {
            delete_rb_tree(node->left);
            delete_rb_tree(node->right);
            delete node;
        }
    }
};
