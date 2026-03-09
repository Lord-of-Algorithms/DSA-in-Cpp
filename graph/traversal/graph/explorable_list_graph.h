#pragma once

#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "explorable_graph.h"

namespace graph {

/**
 * Implements ExplorableGraph using an adjacency list.
 * Edges are stored in both directions so the graph is treated as undirected.
 */
class ExplorableListGraph : public ExplorableGraph {
public:
    ExplorableListGraph() = default;

    void add_vertex(const Vertex& vertex) override {
        adjacency_list_.emplace(vertex, std::vector<Vertex>{});
    }

    /**
     * Adds an undirected edge. Both directions are stored since traversal
     * algorithms may reach each vertex from any connected side.
     */
    void set_edge(const Vertex& source, const Vertex& destination) override {
        if (source == destination) {
            throw std::invalid_argument("Cannot add a self-loop.");
        }
        if (!adjacency_list_.count(source) || !adjacency_list_.count(destination)) {
            throw std::invalid_argument("Both vertices must exist in the graph before connecting them.");
        }
        adjacency_list_.at(source).push_back(destination);
        adjacency_list_.at(destination).push_back(source);
    }

    std::vector<Vertex> get_neighbors(const Vertex& vertex) const override {
        return adjacency_list_.at(vertex);
    }

private:
    std::unordered_map<Vertex, std::vector<Vertex>> adjacency_list_;
};

} // namespace graph
