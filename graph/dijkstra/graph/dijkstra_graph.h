#pragma once

#include <climits>
#include <vector>
#include "../../vertex.h"

namespace graph {

/**
 * Abstract base class defining the interface for a graph that can be used with Dijkstra's algorithm.
 */
class DijkstraGraph {
public:
    // Represents infinite distance — half of INT_MAX to avoid arithmetic overflow
    static constexpr int INFINITY_DIST = INT_MAX / 2;

    virtual void add_vertex(const Vertex& vertex) = 0;

    /**
     * Adds or updates a directed edge. Weight must be non-negative.
     */
    virtual void set_edge(const Vertex& source, const Vertex& destination, int weight) = 0;

    /**
     * Returns all vertices in the graph.
     */
    virtual std::vector<Vertex> get_vertices() const = 0;

    /**
     * Returns all vertices directly reachable from the given vertex.
     */
    virtual std::vector<Vertex> get_neighbors(const Vertex& vertex) const = 0;

    /**
     * Returns the weight of the edge between source and destination,
     * or INFINITY_DIST if no edge exists.
     */
    virtual int get_edge_weight_between(const Vertex& source, const Vertex& destination) const = 0;

    /**
     * Returns the number of vertices in the graph.
     */
    virtual int get_vertex_count() const = 0;

    virtual ~DijkstraGraph() = default;
};

} // namespace graph
