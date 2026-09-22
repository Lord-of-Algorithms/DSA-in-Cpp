#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "b_tree.h"

/**
 * Draws a BTree on the console. Cannot be instantiated; all methods are
 * static.
 *
 * The drawing puts every node of a level on one line and joins each node to
 * its children with a bar, so that the shape of the tree — how wide the
 * nodes are and how they divide the keys between them — can be read at a
 * glance:
 *
 *                      [50]
 *                        |
 *             +---------------------+
 *          [20 35]                [65]
 *             |                     |
 *    +--------+--------+        +-------+
 * [10 15]  [25 30]  [40 45]  [55 60]  [70]
 *
 * A wide tree does not fit a console, so print() falls back to outline(),
 * which spends one line per node and marks the levels by indentation
 * instead:
 *
 * [50]
 *     [20 35]
 *         [10 15]
 *         [25 30]
 *         [40 45]
 *     [65]
 *         [55 60]
 *         [70]
 *
 * An empty tree is a single node holding no keys, and both views show it
 * as [].
 */
class BTreePrinter {
public:
    BTreePrinter() = delete;

    /**
     * Prints the tree — drawn if the drawing fits the console, as an
     * outline if it does not — and a blank line after it.
     */
    static void print(const BTree& tree) {
        std::string drawing = draw(tree);
        if (width_of(drawing) > kMaxWidth) {
            std::cout << "(too wide to draw — the outline instead)\n"
                      << outline(tree);
        } else {
            std::cout << drawing;
        }
        std::cout << "\n";
    }

    /**
     * Returns the tree drawn level by level, each node joined to its
     * children by a bar. The drawing is as wide as it needs to be.
     */
    static std::string draw(const BTree& tree) {
        std::string out;
        for (const std::string& line : layout(tree.root_.get()).lines) {
            out += strip_end(line) + "\n";
        }
        return out;
    }

    /**
     * Returns the tree with one node per line, each child indented under
     * its parent. Unlike draw(), this stays narrow however large the tree
     * grows.
     */
    static std::string outline(const BTree& tree) {
        std::string out;
        append_outline(out, tree.root_.get(), 0);
        return out;
    }

private:
    // Columns between two neighbouring subtrees.
    static constexpr int kGap = 2;

    // The widest drawing print() will show. Beyond it the lines wrap around
    // the console and the picture is worth less than the outline.
    static constexpr int kMaxWidth = 110;

    static constexpr char kBlank = ' ';

    // A run of blanks, as wide as the columns it has to fill.
    static std::string blanks(int count) {
        return std::string(static_cast<std::size_t>(count), kBlank);
    }

    using Node = BTree::Node;

    /**
     * A subtree as it will be drawn: the lines it occupies, the width they
     * share, and the column its own root is centred on.
     */
    struct Block {
        std::vector<std::string> lines;
        int width = 0;
        int center = 0;

        // The given line, padded to the block's width so that whatever is
        // drawn to the right of it starts at the same column on every line.
        std::string line(int row) const {
            std::string out = row < static_cast<int>(lines.size())
                ? lines[static_cast<std::size_t>(row)]
                : std::string();
            int missing = width - static_cast<int>(out.size());
            return missing > 0 ? out + blanks(missing) : out;
        }
    };

    static void append_outline(std::string& out, const Node* node, int depth) {
        out += blanks(depth * 4) + label(node) + "\n";
        for (const auto& child : node->children) {
            append_outline(out, child.get(), depth + 1);
        }
    }

    // A node the way it is drawn: its keys in ascending order, in brackets.
    static std::string label(const Node* node) {
        std::string out = "[";
        for (std::size_t i = 0; i < node->keys.size(); i++) {
            if (i > 0) {
                out += kBlank;
            }
            out += std::to_string(node->keys[i]);
        }
        return out + "]";
    }

    /**
     * Lays out the subtree rooted at the node: the children first, side by
     * side, and then the node itself above the middle of them.
     */
    static Block layout(const Node* node) {
        std::string text = label(node);
        int text_width = static_cast<int>(text.size());
        if (node->is_leaf()) {
            return Block{{text}, text_width, text_width / 2};
        }

        // The children stand in a row, a gap apart. Each one is drawn from
        // the column its own block starts at, so its root ends up at that
        // column plus the block's centre.
        std::vector<Block> children;
        std::vector<int> centers;
        int children_width = 0;
        for (const auto& child : node->children) {
            Block block = layout(child.get());
            if (children_width > 0) {
                children_width += kGap;
            }
            centers.push_back(children_width + block.center);
            children_width += block.width;
            children.push_back(std::move(block));
        }

        // The label sits above the middle child, or halfway between the
        // two middle ones when the count is even — that keeps it over the
        // middle of the row however wide the children turn out to be. A
        // label wider than that middle would hang off the left edge, so
        // everything below it moves right instead.
        std::size_t middle = centers.size() / 2;
        int center = centers.size() % 2 == 1
            ? centers[middle]
            : (centers[middle - 1] + centers[middle]) / 2;
        int shift = std::max(0, text_width / 2 - center);
        center += shift;
        int label_start = center - text_width / 2;
        int width = std::max(shift + children_width, label_start + text_width);

        Block block;
        block.width = width;
        block.center = center;
        block.lines.push_back(blanks(label_start) + text);
        block.lines.push_back(blanks(center) + "|");
        block.lines.push_back(bar(centers, shift, width));
        for (std::string& line : side_by_side(children, shift)) {
            block.lines.push_back(std::move(line));
        }
        return block;
    }

    /**
     * Builds the line that joins a node to its children: a bar reaching
     * from the first child to the last, with a + where each child hangs
     * from it. The node above meets the bar where its own line comes down.
     */
    static std::string bar(const std::vector<int>& centers, int shift,
                           int width) {
        int first = centers.front() + shift;
        int last = centers.back() + shift;
        std::string line = blanks(width);
        for (int i = first; i <= last; i++) {
            line[static_cast<std::size_t>(i)] = '-';
        }
        for (int child_center : centers) {
            line[static_cast<std::size_t>(child_center + shift)] = '+';
        }
        return line;
    }

    /**
     * Puts the children's blocks next to each other, a gap apart, shifted
     * as far right as the node above them needs. The blocks may differ in
     * height only when the tree is broken, and a short one is then filled
     * out with blank lines.
     */
    static std::vector<std::string> side_by_side(
        const std::vector<Block>& children, int shift) {
        int height = 0;
        for (const Block& child : children) {
            height = std::max(height, static_cast<int>(child.lines.size()));
        }
        std::vector<std::string> lines;
        for (int row = 0; row < height; row++) {
            std::string line = blanks(shift);
            for (std::size_t i = 0; i < children.size(); i++) {
                if (i > 0) {
                    line += blanks(kGap);
                }
                line += children[i].line(row);
            }
            lines.push_back(std::move(line));
        }
        return lines;
    }

    static std::string strip_end(const std::string& line) {
        std::size_t end = line.size();
        while (end > 0 && line[end - 1] == kBlank) {
            end--;
        }
        return line.substr(0, end);
    }

    static int width_of(const std::string& drawing) {
        int width = 0;
        std::size_t start = 0;
        while (start < drawing.size()) {
            std::size_t end = drawing.find('\n', start);
            if (end == std::string::npos) {
                end = drawing.size();
            }
            width = std::max(width, static_cast<int>(end - start));
            start = end + 1;
        }
        return width;
    }
};
