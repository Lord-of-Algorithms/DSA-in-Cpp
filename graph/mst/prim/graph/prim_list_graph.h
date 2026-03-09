#pragma once

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "prim_graph.h"

namespace graph {

/**
 * Implements PrimGraph using an adjacency list.
 * Edges are stored in both directions so the graph is treated as undirected.
 */
class PrimListGraph : public PrimGraph {
public:
    PrimListGraph() = default;

    /**
     * Adds a vertex to the graph. Initialises an empty edge list for it.
     */
    void add_vertex(const Vertex& vertex) override {
        adjacency_list_.emplace(vertex, std::vector<Edge>{});
    }

    /**
     * Adds or updates an undirected edge between source and destination.
     * Both directions are stored since Prim's algorithm explores neighbours
     * from whichever side the tree reaches first.
     */
    void set_edge(const Vertex& source, const Vertex& destination, int weight) override {
        if (source == destination) {
            throw std::invalid_argument("Cannot add a self-loop.");
        }
        if (!adjacency_list_.count(source) || !adjacency_list_.count(destination)) {
            throw std::invalid_argument("Both vertices must exist in the graph before connecting them.");
        }
        replace_or_add(adjacency_list_.at(source),      source,      destination, weight);
        replace_or_add(adjacency_list_.at(destination), destination, source,      weight);
    }

    std::vector<Edge> get_edges_for_source(const Vertex& source) const override {
        return adjacency_list_.at(source);
    }

    bool contains_vertex(const Vertex& vertex) const override {
        return adjacency_list_.count(vertex) > 0;
    }

    int get_vertex_count() const override {
        return static_cast<int>(adjacency_list_.size());
    }

private:
    std::unordered_map<Vertex, std::vector<Edge>> adjacency_list_;

    /**
     * Replaces an existing edge between source and destination, or adds a new one.
     */
    static void replace_or_add(std::vector<Edge>& edges,
                                const Vertex& source, const Vertex& destination, int weight) {
        auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& e) {
            return e.source() == source && e.destination() == destination;
        });
        if (it != edges.end()) edges.erase(it);
        edges.emplace_back(source, destination, weight);
    }
};

} // namespace graph
