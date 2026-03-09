#pragma once

#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../../vertex.h"

namespace graph {

/**
 * Implements the union-find (disjoint set union) data structure.
 * Uses path compression in find() and union by rank in union_sets()
 * for near-linear overall time complexity.
 */
class UnionFind {
public:
    /**
     * Constructs a union-find structure from a list of vertices.
     * Each vertex is initially its own representative with rank 0.
     */
    explicit UnionFind(const std::vector<Vertex>& vertices) {
        for (const auto& v : vertices) {
            parent_.insert({v, v});
            rank_.insert({v, 0});
        }
    }

    /**
     * Returns the representative of the set containing the given vertex.
     * Applies path compression so future lookups are faster.
     */
    Vertex find(const Vertex& vertex) {
        if (vertex != parent_.at(vertex)) {
            parent_.insert_or_assign(vertex, find(parent_.at(vertex)));
        }
        return parent_.at(vertex);
    }

    /**
     * Merges the sets that contain vertices x and y.
     * Uses union by rank to keep the tree shallow.
     *
     * Note: 'union' is a reserved keyword in C++, so this method is named union_sets.
     */
    void union_sets(const Vertex& x, const Vertex& y) {
        Vertex root_x = find(x);
        Vertex root_y = find(y);
        if (root_x == root_y) return;

        int rank_x = rank_.at(root_x);
        int rank_y = rank_.at(root_y);

        if (rank_x < rank_y) {
            parent_.insert_or_assign(root_x, root_y);
        } else if (rank_x > rank_y) {
            parent_.insert_or_assign(root_y, root_x);
        } else {
            parent_.insert_or_assign(root_y, root_x);
            rank_.insert_or_assign(root_x, rank_x + 1);
        }
    }

private:
    std::unordered_map<Vertex, Vertex> parent_;
    std::unordered_map<Vertex, int>    rank_;
};

} // namespace graph
