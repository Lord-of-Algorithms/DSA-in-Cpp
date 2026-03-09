#pragma once

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "dijkstra_graph.h"
#include "../../edge.h"

namespace graph {

/**
 * Implements DijkstraGraph using an adjacency list.
 * Efficient for sparse graphs where the number of edges is much smaller than V².
 */
class DijkstraListGraph : public DijkstraGraph {
public:
    DijkstraListGraph() = default;

    void add_vertex(const Vertex& vertex) override {
        if (adjacency_list_.find(vertex) == adjacency_list_.end()) {
            adjacency_list_[vertex] = {};
            vertices_.push_back(vertex);
        }
    }

    /**
     * Adds or updates a directed edge from source to destination.
     * @throws std::invalid_argument if vertices are the same, missing, or weight is negative.
     */
    void set_edge(const Vertex& source, const Vertex& destination, int weight) override {
        if (source == destination) {
            throw std::invalid_argument("Cannot add an edge from a vertex to itself.");
        }
        if (adjacency_list_.find(source) == adjacency_list_.end() ||
            adjacency_list_.find(destination) == adjacency_list_.end()) {
            throw std::invalid_argument("One or both vertices do not exist in the graph.");
        }
        if (weight < 0) {
            throw std::invalid_argument("Edge weight cannot be negative.");
        }

        auto& edges = adjacency_list_.at(source);

        // Remove existing edge to the same destination, if any
        edges.erase(
            std::remove_if(edges.begin(), edges.end(),
                [&](const Edge& e) { return e.destination() == destination; }),
            edges.end());

        edges.emplace_back(source, destination, weight);
    }

    std::vector<Vertex> get_vertices() const override {
        return vertices_;
    }

    std::vector<Vertex> get_neighbors(const Vertex& vertex) const override {
        std::vector<Vertex> neighbors;
        for (const auto& edge : adjacency_list_.at(vertex)) {
            neighbors.push_back(edge.destination());
        }
        return neighbors;
    }

    int get_edge_weight_between(const Vertex& source, const Vertex& destination) const override {
        for (const auto& edge : adjacency_list_.at(source)) {
            if (edge.destination() == destination) {
                return edge.weight();
            }
        }
        return INFINITY_DIST;
    }

    int get_vertex_count() const override {
        return static_cast<int>(vertices_.size());
    }

private:
    std::unordered_map<Vertex, std::vector<Edge>> adjacency_list_;
    std::vector<Vertex>                           vertices_;
};

} // namespace graph
