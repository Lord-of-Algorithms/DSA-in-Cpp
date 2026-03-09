#pragma once

namespace graph {

/**
 * Enumerates the supported graph traversal methods.
 */
enum class TraversalMethod {
    /** Depth-First Search using an iterative approach with an explicit stack. */
    DFS,
    /** Depth-First Search using a recursive approach. */
    DFS_RECURSIVE,
    /** Breadth-First Search. */
    BFS
};

} // namespace graph
