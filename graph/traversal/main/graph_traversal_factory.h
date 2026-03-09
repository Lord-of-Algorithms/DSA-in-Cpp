#pragma once

#include <memory>
#include <stdexcept>

#include "traversal_method.h"
#include "../algorithms/graph_traversal.h"
#include "../algorithms/bfs_traversal.h"
#include "../algorithms/dfs_traversal.h"
#include "../algorithms/dfs_recursive_traversal.h"

namespace graph {

/**
 * Creates a GraphTraversal instance for the requested traversal method.
 */
class GraphTraversalFactory {
public:
    GraphTraversalFactory() = delete;

    static std::unique_ptr<GraphTraversal> create_traversal(TraversalMethod method) {
        switch (method) {
            case TraversalMethod::DFS:
                return std::make_unique<DfsTraversal>();
            case TraversalMethod::DFS_RECURSIVE:
                return std::make_unique<DfsRecursiveTraversal>();
            case TraversalMethod::BFS:
                return std::make_unique<BfsTraversal>();
            default:
                throw std::invalid_argument("Unknown traversal method.");
        }
    }
};

} // namespace graph
