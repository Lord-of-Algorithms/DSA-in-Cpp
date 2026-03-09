#pragma once

#include <stack>
#include <string>

#include "base_graph_traversal.h"

namespace graph {

/**
 * Implements iterative Depth-First Search (DFS) traversal using an explicit stack.
 * Explores as deep as possible along each branch before backtracking.
 */
class DfsTraversal : public BaseGraphTraversal {
public:
    void traverse(ExplorableGraph& graph, const Vertex& start_vertex) override {
        std::stack<Vertex> stack;
        visit(start_vertex);
        stack.push(start_vertex);

        while (!stack.empty()) {
            std::optional<Vertex> unvisited = find_unvisited_neighbor(graph, stack.top());
            if (!unvisited.has_value()) {
                stack.pop();
            } else {
                visit(*unvisited);
                stack.push(*unvisited);
            }
        }
    }

    std::string name() const override { return "DFS"; }
};

} // namespace graph
