#pragma once

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "../vertex.h"
#include "graph/dijkstra_graph.h"
#include "vertex_distance_priority_queue.h"

namespace graph {

/**
 * Implements Dijkstra's algorithm for finding the shortest paths from a single source
 * vertex to all other vertices in a graph with non-negative edge weights.
 */
class DijkstraAlgorithm {
public:
    DijkstraAlgorithm() = default;

    /**
     * Computes the shortest paths from the specified source vertex.
     * Results are stored internally and accessible via get_shortest_path_to().
     *
     * @throws std::invalid_argument if the source vertex is not in the graph.
     */
    void compute_paths(DijkstraGraph& graph, const Vertex& source) {
        auto vertices = graph.get_vertices();

        if (std::find(vertices.begin(), vertices.end(), source) == vertices.end()) {
            throw std::invalid_argument("Source vertex is not in the graph.");
        }

        std::unordered_map<Vertex, int> distance_map;
        VertexDistancePriorityQueue pq(graph.get_vertex_count());

        for (const auto& v : vertices) {
            int initial_distance = (v == source) ? 0 : DijkstraGraph::INFINITY_DIST;
            distance_map[v]    = initial_distance;
            predecessor_map_[v] = std::nullopt;
            pq.add(v, initial_distance);
        }

        while (!pq.is_empty()) {
            Vertex u = pq.poll_smallest();

            for (const auto& neighbor : graph.get_neighbors(u)) {
                int current_dist     = distance_map.at(neighbor);
                int edge_weight      = graph.get_edge_weight_between(u, neighbor);
                int alternative_dist = distance_map.at(u) + edge_weight;

                if (alternative_dist < current_dist) {
                    pq.update(neighbor, alternative_dist);
                    distance_map[neighbor]    = alternative_dist;
                    predecessor_map_[neighbor] = u;
                }
            }
        }
    }

    /**
     * Returns the shortest path from the source to the given target vertex.
     * Must be called after compute_paths().
     */
    std::vector<Vertex> get_shortest_path_to(const Vertex& target) const {
        std::vector<Vertex> path;
        std::optional<Vertex> current = target;
        while (current.has_value()) {
            path.push_back(*current);
            auto it = predecessor_map_.find(*current);
            current = (it != predecessor_map_.end()) ? it->second : std::nullopt;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    /**
     * Clears stored predecessors so the algorithm can be rerun on a modified graph.
     */
    void reset_state() {
        predecessor_map_.clear();
    }

private:
    std::unordered_map<Vertex, std::optional<Vertex>> predecessor_map_;
};

} // namespace graph
