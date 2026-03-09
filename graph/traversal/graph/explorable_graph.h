#pragma once

#include <vector>
#include "../../vertex.h"

namespace graph {

/**
 * Abstract base class for an undirected, unweighted graph
 * that supports neighbor lookup — the key operation needed by traversal algorithms.
 */
class ExplorableGraph {
public:
    /**
     * Adds a vertex to the graph.
     */
    virtual void add_vertex(const Vertex& vertex) = 0;

    /**
     * Adds an undirected edge between source and destination.
     * Both directions are stored so the graph can be explored from either end.
     */
    virtual void set_edge(const Vertex& source, const Vertex& destination) = 0;

    /**
     * Returns all vertices directly reachable from the given vertex.
     */
    virtual std::vector<Vertex> get_neighbors(const Vertex& vertex) const = 0;

    virtual ~ExplorableGraph() = default;
};

} // namespace graph
