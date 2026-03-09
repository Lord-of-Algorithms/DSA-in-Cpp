#pragma once

namespace graph {

/**
 * Specifies the internal representation used to store graph edges.
 */
enum class GraphRepresentation {
    LIST,   // Adjacency list — efficient for sparse graphs
    MATRIX  // Adjacency matrix — efficient for dense graphs and O(1) edge lookups
};

} // namespace graph
