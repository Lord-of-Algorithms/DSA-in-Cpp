#pragma once

#include <queue>
#include <string>

#include "base_graph_traversal.h"

namespace graph {

/**
 * Implements Breadth-First Search (BFS) traversal.
 * Explores all neighbours of the current vertex before moving deeper,
 * resulting in a level-by-level exploration of the graph.
 */
class BfsTraversal : public BaseGraphTraversal {
public:
    void traverse(ExplorableGraph& graph, const Vertex& start_vertex) override {
        std::queue<Vertex> queue;
        visit(start_vertex);
        queue.push(start_vertex);

        while (!queue.empty()) {
            Vertex head = queue.front();
            queue.pop();

            std::optional<Vertex> unvisited;
            while ((unvisited = find_unvisited_neighbor(graph, head)).has_value()) {
                visit(*unvisited);
                queue.push(*unvisited);
            }
        }
    }

    std::string name() const override { return "BFS"; }
};

} // namespace graph
