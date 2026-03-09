#pragma once

#include <stdexcept>
#include <vector>

/**
 * Implements the union-find data structure (also known as disjoint-set union).
 *
 * Elements are represented as integers in the range [0, count).
 * Uses path compression in find() and union by rank in union_sets()
 * for near-linear overall time complexity.
 *
 * Note: 'union' is a reserved keyword in C++, so the merge operation
 * is named union_sets() instead.
 */
class UnionFind {
public:
    /**
     * Initialises the structure with the given number of elements.
     * Each element starts as its own representative with rank 0.
     */
    explicit UnionFind(int count) {
        sets_.reserve(count);
        for (int i = 0; i < count; ++i) {
            sets_.push_back({i, 0});
        }
    }

    /**
     * Returns the representative (root) of the set containing element.
     * Applies path compression so future lookups on the same path are O(1).
     *
     * @throws std::out_of_range if element is out of bounds.
     */
    int find(int element) {
        if (element < 0 || element >= static_cast<int>(sets_.size())) {
            throw std::out_of_range("Element out of bounds.");
        }
        if (sets_[element].parent != element) {
            sets_[element].parent = find(sets_[element].parent);
        }
        return sets_[element].parent;
    }

    /**
     * Merges the sets containing elements x and y.
     * Uses union by rank to keep the tree shallow.
     *
     * @throws std::out_of_range if either element is out of bounds.
     */
    void union_sets(int x, int y) {
        if (x < 0 || x >= static_cast<int>(sets_.size()) ||
            y < 0 || y >= static_cast<int>(sets_.size())) {
            throw std::out_of_range("Element out of bounds.");
        }

        int x_root = find(x);
        int y_root = find(y);

        if (x_root == y_root) return;

        if (sets_[x_root].rank < sets_[y_root].rank) {
            sets_[x_root].parent = y_root;
        } else if (sets_[x_root].rank > sets_[y_root].rank) {
            sets_[y_root].parent = x_root;
        } else {
            sets_[y_root].parent = x_root;
            ++sets_[x_root].rank;
        }
    }

private:
    struct Set {
        int parent;
        int rank;
    };

    std::vector<Set> sets_;
};
