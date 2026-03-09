#pragma once

#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "dijkstra_graph.h"

namespace graph {

/**
 * Implements DijkstraGraph using an adjacency matrix.
 * Efficient for dense graphs and provides O(1) edge weight lookups.
 */
class DijkstraMatrixGraph : public DijkstraGraph {
public:
    /**
     * Constructs a matrix graph with a fixed maximum number of vertices.
     * All entries are initialised to INFINITY_DIST, indicating no edges.
     */
    explicit DijkstraMatrixGraph(int max_vertex_count)
        : max_vertices_(max_vertex_count),
          adjacency_matrix_(max_vertex_count, std::vector<int>(max_vertex_count, INFINITY_DIST)) {
        if (max_vertex_count <= 0) {
            throw std::invalid_argument("Maximum vertex count must be greater than 0.");
        }
    }

    void add_vertex(const Vertex& vertex) override {
        if (static_cast<int>(vertices_.size()) >= max_vertices_) {
            throw std::overflow_error("Maximum vertex limit reached.");
        }
        if (indices_map_.find(vertex) == indices_map_.end()) {
            indices_map_[vertex] = static_cast<int>(vertices_.size());
            vertices_.push_back(vertex);
        }
    }

    /**
     * Adds or updates a directed edge in the matrix.
     * @throws std::invalid_argument if vertices are the same, missing, or weight is negative.
     */
    void set_edge(const Vertex& source, const Vertex& destination, int weight) override {
        if (source == destination) {
            throw std::invalid_argument("Cannot add an edge from a vertex to itself.");
        }
        if (indices_map_.find(source) == indices_map_.end()) {
            throw std::invalid_argument("Source vertex does not exist in the graph.");
        }
        if (indices_map_.find(destination) == indices_map_.end()) {
            throw std::invalid_argument("Destination vertex does not exist in the graph.");
        }
        if (weight < 0) {
            throw std::invalid_argument("Edge weight cannot be negative.");
        }

        adjacency_matrix_[indices_map_.at(source)][indices_map_.at(destination)] = weight;
    }

    std::vector<Vertex> get_vertices() const override {
        return vertices_;
    }

    std::vector<Vertex> get_neighbors(const Vertex& vertex) const override {
        int index = indices_map_.at(vertex);
        std::vector<Vertex> neighbors;
        for (int i = 0; i < static_cast<int>(vertices_.size()); ++i) {
            if (adjacency_matrix_[index][i] != INFINITY_DIST) {
                neighbors.push_back(vertices_[i]);
            }
        }
        return neighbors;
    }

    int get_edge_weight_between(const Vertex& source, const Vertex& destination) const override {
        return adjacency_matrix_[indices_map_.at(source)][indices_map_.at(destination)];
    }

    int get_vertex_count() const override {
        return static_cast<int>(vertices_.size());
    }

private:
    int                                max_vertices_;
    std::unordered_map<Vertex, int>    indices_map_;
    std::vector<std::vector<int>>      adjacency_matrix_;
    std::vector<Vertex>                vertices_;
};

} // namespace graph
