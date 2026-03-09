#pragma once

#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "explorable_graph.h"

namespace graph {

/**
 * Implements ExplorableGraph using an adjacency matrix.
 * Matrix cells hold 1 for connected and 0 for no edge.
 * Edges are stored symmetrically so the graph is treated as undirected.
 */
class ExplorableMatrixGraph : public ExplorableGraph {
public:
    explicit ExplorableMatrixGraph(int max_vertex_count)
        : max_vertices_(max_vertex_count),
          current_count_(0),
          matrix_(max_vertex_count, std::vector<int>(max_vertex_count, 0)) {}

    void add_vertex(const Vertex& vertex) override {
        if (current_count_ >= max_vertices_) {
            throw std::overflow_error("Maximum vertex count reached.");
        }
        if (!index_map_.count(vertex)) {
            vertices_.push_back(vertex);
            index_map_.emplace(vertex, current_count_++);
        }
    }

    void set_edge(const Vertex& source, const Vertex& destination) override {
        if (source == destination) {
            throw std::invalid_argument("Cannot add a self-loop.");
        }
        if (!index_map_.count(source)) {
            throw std::invalid_argument("Source vertex does not exist in the graph.");
        }
        if (!index_map_.count(destination)) {
            throw std::invalid_argument("Destination vertex does not exist in the graph.");
        }
        int s = index_map_.at(source);
        int d = index_map_.at(destination);
        matrix_[s][d] = 1;
        matrix_[d][s] = 1;
    }

    std::vector<Vertex> get_neighbors(const Vertex& vertex) const override {
        int idx = index_map_.at(vertex);
        std::vector<Vertex> neighbors;
        for (int i = 0; i < current_count_; ++i) {
            if (matrix_[idx][i] == 1) {
                neighbors.push_back(vertices_[i]);
            }
        }
        return neighbors;
    }

private:
    int max_vertices_;
    int current_count_;
    std::vector<std::vector<int>>   matrix_;
    std::unordered_map<Vertex, int> index_map_;
    std::vector<Vertex>             vertices_;  // index → vertex lookup
};

} // namespace graph
