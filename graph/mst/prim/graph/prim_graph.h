#pragma once

#include <vector>
#include "../../../edge.h"
#include "../../../vertex.h"

namespace graph {

/**
 * Abstract base class defining the interface for a graph used with Prim's algorithm.
 * Provides methods for building an undirected weighted graph and querying it
 * in the way Prim's algorithm needs: edges by source vertex and vertex membership.
 */
class PrimGraph {
public:
    /**
     * Adds a vertex to the graph.
     */
    virtual void add_vertex(const Vertex& vertex) = 0;

    /**
     * Adds or updates an undirected edge between two vertices with the given weight.
     * Both directions are stored so the graph can be traversed from either end.
     */
    virtual void set_edge(const Vertex& source, const Vertex& destination, int weight) = 0;

    /**
     * Returns all edges originating from the given vertex.
     */
    virtual std::vector<Edge> get_edges_for_source(const Vertex& source) const = 0;

    /**
     * Returns true if the vertex is present in the graph.
     */
    virtual bool contains_vertex(const Vertex& vertex) const = 0;

    /**
     * Returns the total number of vertices in the graph.
     */
    virtual int get_vertex_count() const = 0;

    virtual ~PrimGraph() = default;
};

} // namespace graph
