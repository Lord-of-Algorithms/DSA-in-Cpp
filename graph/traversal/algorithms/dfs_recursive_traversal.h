#pragma once

#include <string>

#include "base_graph_traversal.h"

namespace graph {

/**
 * Implements recursive Depth-First Search (DFS) traversal.
 * Functionally identical to DfsTraversal but uses the call stack for backtracking
 * instead of an explicit stack — making the algorithm structure more visible.
 */
class DfsRecursiveTraversal : public BaseGraphTraversal {
public:
    void traverse(ExplorableGraph& graph, const Vertex& start_vertex) override {
        recursive_dfs(graph, start_vertex);
    }

    std::string name() const override { return "Recursive DFS"; }

private:
    /**
     * Visits the given vertex then recursively visits all its unvisited neighbours.
     */
    void recursive_dfs(ExplorableGraph& graph, const Vertex& vertex) {
        visit(vertex);

        std::optional<Vertex> unvisited;
        while ((unvisited = find_unvisited_neighbor(graph, vertex)).has_value()) {
            recursive_dfs(graph, *unvisited);
        }
    }
};

} // namespace graph
