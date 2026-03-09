#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "tree_node.h"

/**
 * Enumeration for the number of digits used to display each node key.
 */
enum class NumberDigit {
    DoubleDigit = 2,
    ThreeDigit  = 3,
    FourDigit   = 4
};

/**
 * Prints a binary tree to stdout in a human-readable, graphical format.
 * Cannot be instantiated; all methods are static.
 *
 * Ported from the Java version by github.com/Lord-of-Algorithms (Apache 2.0).
 */
class BinaryTreePrinter {
public:
    BinaryTreePrinter() = delete;

    /**
     * Prints the binary tree using the default settings (2-digit keys, max 6 levels).
     */
    static void print(TreeNode* root) {
        print(root, NumberDigit::DoubleDigit, kMaxLevel);
    }

    /**
     * Prints the binary tree with custom digit width and max level.
     */
    static void print(TreeNode* root, NumberDigit key_digit, int max_tree_level) {
        if (root == nullptr) {
            std::cout << "The tree is empty.\n";
            return;
        }

        key_node_value_digits_ = static_cast<int>(key_digit);
        calculate_valid_max_node_key_value(key_digit);

        int max_level = get_tree_max_level(root);
        if (max_level > max_tree_level) {
            throw std::runtime_error(
                "\n\n\n==========================================\n"
                "Max level of the binary tree that can be printed is " +
                std::to_string(max_tree_level) + ". Current level is " +
                std::to_string(max_level) + ".\n==========================================\n\n\n");
        }
        if (max_level == max_tree_level) {
            // Edge case: reduce by 1 to make the tree fit
            max_level -= 1;
        }

        // In-order traversal preserves left-to-right node order for each level
        std::vector<TreeNode*>              inorder_nodes;
        std::unordered_map<TreeNode*, int>  level_map;
        fill_level_map(root, 0, inorder_nodes, level_map);

        std::unordered_map<TreeNode*, int> start_margin_map;
        fill_node_start_margin_map(root, level_map, true, 0, false, start_margin_map, max_level - 1);

        std::cout << "\n";
        print_binary_tree(root, max_level - 1, start_margin_map, level_map, inorder_nodes);
        std::cout << "\n\n";
    }

private:
    static constexpr int  kMaxLevel         = 6;
    static constexpr int  kMinNodeKeyValue  = 0;
    static constexpr char kBlank            = ' ';

    // Mutable static state (mirrors Java's static fields)
    inline static int max_node_key_value_      = 99;
    inline static int key_node_value_digits_   = 2;

    // ─── Helpers ─────────────────────────────────────────────────────────────

    static void calculate_valid_max_node_key_value(NumberDigit d) {
        switch (d) {
            case NumberDigit::ThreeDigit: max_node_key_value_ = 999;  break;
            case NumberDigit::FourDigit:  max_node_key_value_ = 9999; break;
            default:                      max_node_key_value_ = 99;
        }
    }

    static int get_tree_max_level(TreeNode* node) {
        if (node == nullptr) return 0;
        return std::max(get_tree_max_level(node->get_left()),
                        get_tree_max_level(node->get_right())) + 1;
    }

    /**
     * In-order traversal: associates each node with its level.
     * Uses a vector to preserve left-to-right insertion order (mirrors LinkedHashMap).
     */
    static void fill_level_map(
        TreeNode*                           root,
        int                              level,
        std::vector<TreeNode*>&             inorder_nodes,
        std::unordered_map<TreeNode*, int>& level_map)
    {
        if (root == nullptr) return;

        if (root->get_key() < kMinNodeKeyValue || root->get_key() > max_node_key_value_) {
            throw std::runtime_error(
                "\n\n\n==========================================\n"
                "The key of the node must be >= " + std::to_string(kMinNodeKeyValue) +
                " and <= " + std::to_string(max_node_key_value_) +
                "\n==========================================\n\n\n");
        }

        fill_level_map(root->get_left(), level + 1, inorder_nodes, level_map);
        inorder_nodes.push_back(root);
        level_map[root] = level;
        fill_level_map(root->get_right(), level + 1, inorder_nodes, level_map);
    }

    /**
     * Pre-order: calculates start margin (x-position) for each node.
     */
    static void fill_node_start_margin_map(
        TreeNode*                           root,
        std::unordered_map<TreeNode*, int>& level_map,
        bool                             is_root,
        int                              start_margin,
        bool                             is_left_child,
        std::unordered_map<TreeNode*, int>& margin_map,
        int                              max_level)
    {
        if (root == nullptr) return;

        int node_level         = level_map.at(root);
        int space_between      = key_node_value_digits_ *
            (static_cast<int>(std::pow(2.0, max_level - node_level + 1)) - 1);

        if (is_root) {
            start_margin = key_node_value_digits_ *
                (static_cast<int>(std::pow(2.0, max_level - node_level)) - 1);
        } else if (is_left_child) {
            start_margin = start_margin
                - space_between / key_node_value_digits_
                - key_node_value_digits_ / 2;
        } else {
            start_margin = start_margin
                + space_between / key_node_value_digits_
                + key_node_value_digits_ / 2;
        }

        margin_map[root] = start_margin;

        fill_node_start_margin_map(root->get_left(),  level_map, false, start_margin, true,  margin_map, max_level);
        fill_node_start_margin_map(root->get_right(), level_map, false, start_margin, false, margin_map, max_level);
    }

    /**
     * BFS-based printing of nodes, with edge lines between levels.
     */
    static void print_binary_tree(
        TreeNode*                                root,
        int                                   max_level,
        const std::unordered_map<TreeNode*, int>& margin_map,
        const std::unordered_map<TreeNode*, int>& level_map,
        const std::vector<TreeNode*>&             inorder_nodes)
    {
        if (root == nullptr) return;

        std::queue<TreeNode*> queue;
        queue.push(root);
        int current_level    = 0;
        int current_position = 0;

        while (!queue.empty()) {
            TreeNode* node         = queue.front(); queue.pop();
            int    start_margin = margin_map.at(node);
            int    level        = level_map.at(node);

            if (current_level != level) {
                std::cout << "\n";

                int edge_height = static_cast<int>(std::pow(2.0, max_level - current_level)) - 1;
                for (int i = 0; i < edge_height; ++i) {
                    print_edges(current_level, margin_map, level_map, inorder_nodes, i);
                }

                current_level    = level;
                current_position = 0;
            }

            int spaces = start_margin - current_position;
            for (int i = 0; i < spaces; ++i) {
                std::cout << kBlank;
                ++current_position;
            }
            print_node(node);
            current_position += key_node_value_digits_;

            if (node->get_left()  != nullptr) queue.push(node->get_left());
            if (node->get_right() != nullptr) queue.push(node->get_right());
        }
    }

    /**
     * Prints the node's key, padded according to the digit-width setting.
     * Uses node->to_string() so RB nodes can show ANSI red color.
     */
    static void print_node(TreeNode* node) {
        int         key = node->get_key();
        std::string s   = node->to_string();

        if (key_node_value_digits_ == static_cast<int>(NumberDigit::DoubleDigit)) {
            if (key < 10) std::cout << kBlank << s;
            else          std::cout << s;
        } else if (key_node_value_digits_ == static_cast<int>(NumberDigit::ThreeDigit)) {
            if      (key < 10)  std::cout << kBlank << s << kBlank;
            else if (key < 100) std::cout << s << kBlank;
            else                std::cout << s;
        } else {  // FourDigit
            if      (key < 10)   std::cout << kBlank << s << kBlank << kBlank;
            else if (key < 100)  std::cout << kBlank << s << kBlank;
            else if (key < 1000) std::cout << s << kBlank;
            else                 std::cout << s;
        }
    }

    /**
     * Prints one row of edge lines (/ and \) connecting a level to its children.
     */
    static void print_edges(
        int                                    current_level,
        const std::unordered_map<TreeNode*, int>& margin_map,
        const std::unordered_map<TreeNode*, int>& level_map,
        const std::vector<TreeNode*>&             inorder_nodes,
        int                                    iteration)
    {
        // Collect nodes at current_level in in-order (left-to-right) order
        std::vector<TreeNode*> level_nodes;
        for (TreeNode* n : inorder_nodes) {
            if (level_map.at(n) == current_level) {
                level_nodes.push_back(n);
            }
        }

        std::string between_slash_backslash(std::max(0, 2 * iteration), kBlank);

        int current_position = 0;
        for (TreeNode* node : level_nodes) {
            int start_margin   = margin_map.at(node);
            int slash_position = start_margin - current_position - iteration
                                 + (key_node_value_digits_ / 2 - 1);

            for (int i = 0; i < slash_position; ++i) {
                std::cout << kBlank;
                ++current_position;
            }

            char slash      = (node->get_left()  != nullptr) ? '/'  : kBlank;
            char back_slash = (node->get_right() != nullptr) ? '\\' : kBlank;

            std::cout << slash << between_slash_backslash << back_slash;
            current_position += 2 * (iteration + 1);
        }
        std::cout << "\n";
    }
};
