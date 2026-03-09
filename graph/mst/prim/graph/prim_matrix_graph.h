#pragma once

#include <climits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "prim_graph.h"

namespace graph {

/**
 * Implements PrimGraph using an adjacency matrix.
 * Edges are stored symmetrically so the graph is treated as undirected.
 */
class PrimMatrixGraph : public PrimGraph {
public:
    static constexpr int NO_EDGE = INT_MAX;

    /**
     * Constructs the graph with a fixed maximum number of vertices.
     */
    explicit PrimMatrixGraph(int max_vertex_count)
        : max_vertices_(max_vertex_count),
          current_count_(0),
          matrix_(max_vertex_count, std::vector<int>(max_vertex_count, NO_EDGE)) {}

    /**
     * Adds a vertex and assigns it the next available matrix index.
     */
    void add_vertex(const Vertex& vertex) override {
        if (current_count_ >= max_vertices_) {
            throw std::overflow_error("Maximum vertex count reached.");
        }
        if (!index_map_.count(vertex)) {
            vertices_.push_back(vertex);
            index_map_.emplace(vertex, current_count_++);
        }
    }

    /**
     * Adds or updates an undirected edge. Both matrix cells are set symmetrically.
     */
    void set_edge(const Vertex& source, const Vertex& destination, int weight) override {
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
        matrix_[s][d] = weight;
        matrix_[d][s] = weight;
    }

    /**
     * Returns all edges from the given source by scanning its matrix row.
     */
    std::vector<Edge> get_edges_for_source(const Vertex& source) const override {
        int s = index_map_.at(source);
        std::vector<Edge> edges;
        for (int i = 0; i < current_count_; ++i) {
            if (matrix_[s][i] != NO_EDGE) {
                edges.emplace_back(source, vertices_[i], matrix_[s][i]);
            }
        }
        return edges;
    }

    bool contains_vertex(const Vertex& vertex) const override {
        return index_map_.count(vertex) > 0;
    }

    int get_vertex_count() const override {
        return current_count_;
    }

private:
    int max_vertices_;
    int current_count_;
    std::vector<std::vector<int>>  matrix_;
    std::unordered_map<Vertex, int> index_map_;
    std::vector<Vertex>             vertices_;  // index → vertex lookup
};

} // namespace graph
