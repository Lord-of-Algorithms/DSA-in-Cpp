#pragma once

#include <optional>
#include <unordered_set>
#include <vector>

#include "graph_traversal.h"

namespace graph {

/**
 * Base class for graph traversal algorithms.
 * Provides shared state and helpers: visited tracking, path recording,
 * and finding the first unvisited neighbour of a vertex.
 *
 * Concrete subclasses only need to implement traverse() and name().
 */
class BaseGraphTraversal : public GraphTraversal {
public:
    std::vector<Vertex> get_traversal_path() const override {
        return traversal_path_;
    }

    void reset_state() override {
        visited_.clear();
        traversal_path_.clear();
    }

protected:
    /**
     * Marks a vertex as visited and appends it to the traversal path.
     */
    void visit(const Vertex& vertex) {
        visited_.insert(vertex);
        traversal_path_.push_back(vertex);
    }

    bool is_visited(const Vertex& vertex) const {
        return visited_.count(vertex) > 0;
    }

    /**
     * Returns the first unvisited neighbour of the given vertex,
     * or nullopt if all neighbours have already been visited.
     */
    std::optional<Vertex> find_unvisited_neighbor(ExplorableGraph& graph,
                                                   const Vertex& vertex) const {
        for (const auto& neighbor : graph.get_neighbors(vertex)) {
            if (!is_visited(neighbor)) return neighbor;
        }
        return std::nullopt;
    }

private:
    std::unordered_set<Vertex> visited_;
    std::vector<Vertex>        traversal_path_;
};

} // namespace graph
