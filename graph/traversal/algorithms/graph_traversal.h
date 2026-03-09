#pragma once

#include <string>
#include <vector>

#include "../../vertex.h"
#include "../graph/explorable_graph.h"

namespace graph {

/**
 * Defines the interface for graph traversal algorithms.
 */
class GraphTraversal {
public:
    /**
     * Traverses the graph starting from the given vertex.
     */
    virtual void traverse(ExplorableGraph& graph, const Vertex& start_vertex) = 0;

    /**
     * Returns the sequence of vertices visited during the traversal.
     */
    virtual std::vector<Vertex> get_traversal_path() const = 0;

    /**
     * Resets the traversal state so the same instance can be reused.
     */
    virtual void reset_state() = 0;

    /**
     * Returns the human-readable name of the traversal algorithm.
     */
    virtual std::string name() const = 0;

    virtual ~GraphTraversal() = default;
};

} // namespace graph
