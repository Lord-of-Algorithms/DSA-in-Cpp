#pragma once

#include <iostream>
#include <string>
#include "../../tree_node.h"

/**
 * Represents a node in a Red-Black Tree.
 * Contains a key, left/right/parent pointers, color flag, and sentinel flag.
 * Red nodes are displayed with ANSI red color in the tree printer.
 */
class RbNode : public TreeNode {
public:
    int     key    = 0;
    RbNode* left   = nullptr;
    RbNode* right  = nullptr;
    RbNode* parent = nullptr;

    /**
     * Constructor for a regular RbNode.
     * Children and parent are initialized to the given sentinel.
     */
    RbNode(int k, RbNode* sentinel)
        : key(k), left(sentinel), right(sentinel), parent(sentinel) {}

    /**
     * Factory method to create a sentinel (NIL) node.
     * Sentinel nodes are always black and mark null leaves / root's parent.
     */
    static RbNode* create_sentinel_node() {
        RbNode* node      = new RbNode();
        node->is_sentinel_ = true;
        node->set_black();
        return node;
    }

    bool is_red()   const { return !is_black_; }
    bool is_black() const { return is_black_; }
    void set_black()      { is_black_ = true;  }
    void set_red()        { is_black_ = false; }

    // ─── TreeNode interface ─────────────────────────────────────────────────────

    int get_key() const override { return key; }

    // Returns nullptr for sentinel children (matches Java's null check)
    TreeNode* get_left() const override {
        return (left  && !left->is_sentinel_)  ? left  : nullptr;
    }
    TreeNode* get_right() const override {
        return (right && !right->is_sentinel_) ? right : nullptr;
    }

    void visit() const override { std::cout << key << " "; }

    /**
     * Red nodes are shown in ANSI red; black nodes use default color.
     */
    std::string to_string() const override {
        if (is_red()) {
            return "\033[31m" + std::to_string(key) + "\033[0m";
        }
        return std::to_string(key);
    }

private:
    bool is_black_    = false;
    bool is_sentinel_ = false;

    // Private default constructor — used only by create_sentinel_node()
    RbNode() = default;
};
