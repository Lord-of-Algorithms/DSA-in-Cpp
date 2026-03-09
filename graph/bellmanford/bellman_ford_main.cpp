/**
 * Demonstrates the Bellman-Ford algorithm for finding the shortest paths from a single source
 * vertex to all other vertices in a weighted directed graph.
 *
 * Unlike Dijkstra's algorithm, Bellman-Ford correctly handles negative edge weights and can
 * detect negative weight cycles, which make shortest-path distances undefined.
 *
 * Both the algorithm and the demos use Vertex to stay consistent with the rest of the
 * codebase, but this is not a requirement of Bellman-Ford itself. Because the algorithm only holds
 * vertex references as map keys and never inspects their internals, plain integers could be used
 * just as well — replacing unordered_map<Vertex, int> with a simple int[] array.
 *
 * Time complexity: O(V * E), where V is the number of vertices and E is the number of edges.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../vertex.h"
#include "../edge.h"

using namespace graph;

// Represents infinity: half of INT_MAX to avoid arithmetic overflow
static const int INFINITY_DIST = INT_MAX / 2;

// -------------------------------------------------------------------------
// Result holder
// -------------------------------------------------------------------------

/**
 * Holds the output of the Bellman-Ford algorithm: shortest distances from the source,
 * the predecessor map for path reconstruction, and a flag indicating a negative cycle.
 */
struct BellmanFordResult {
    std::unordered_map<Vertex, int>                   distances;
    std::unordered_map<Vertex, std::optional<Vertex>> predecessors;
    bool                                              has_negative_cycle;
};

// -------------------------------------------------------------------------
// Algorithm
// -------------------------------------------------------------------------

/**
 * Computes the shortest paths from a source vertex to all other vertices using the
 * Bellman-Ford algorithm.
 *
 * @param vertices  All vertices in the graph.
 * @param edges     All directed edges in the graph (may include negative weights).
 * @param source    The starting vertex.
 * @return A BellmanFordResult containing distances, predecessors, and a negative-cycle flag.
 */
BellmanFordResult bellman_ford(const std::vector<Vertex>& vertices,
                               const std::vector<Edge>&   edges,
                               const Vertex&              source) {
    std::unordered_map<Vertex, int>                   distances;
    std::unordered_map<Vertex, std::optional<Vertex>> predecessors;

    // Step 1: Initialize distances — source is 0, everything else is infinity
    for (const auto& v : vertices) {
        distances[v]    = (v == source) ? 0 : INFINITY_DIST;
        predecessors[v] = std::nullopt;
    }

    int vertex_count = static_cast<int>(vertices.size());

    // Step 2: Relax all edges V-1 times
    bool updated = false;
    for (int i = 0; i < vertex_count - 1; ++i) {
        updated = false;

        for (const auto& edge : edges) {
            const Vertex& u = edge.source();
            const Vertex& v = edge.destination();
            int dist_u = distances.at(u);

            if (dist_u != INFINITY_DIST) {
                int new_dist = dist_u + edge.weight();
                if (new_dist < distances.at(v)) {
                    distances[v]    = new_dist;
                    predecessors[v] = u;
                    updated = true;
                }
            }
        }

        // Early termination: if no edge was relaxed in this pass, distances are final
        // and no negative cycle exists — Step 3 can be skipped entirely.
        if (!updated) {
            break;
        }
    }

    // Step 3: Detect negative weight cycles.
    // Only run if the loop completed all V-1 passes with updates still happening —
    // meaning distances never stabilised, which is the hallmark of a negative cycle.
    bool has_negative_cycle = false;
    if (updated) {
        for (const auto& edge : edges) {
            const Vertex& u = edge.source();
            const Vertex& v = edge.destination();
            int dist_u = distances.at(u);
            if (dist_u != INFINITY_DIST && dist_u + edge.weight() < distances.at(v)) {
                has_negative_cycle = true;
                break;
            }
        }
    }

    return {distances, predecessors, has_negative_cycle};
}

/**
 * Reconstructs the shortest path from the source to the given target vertex
 * by following the predecessor chain.
 *
 * @param predecessors  The predecessor map produced by bellman_ford.
 * @param target        The destination vertex.
 * @return A list of vertices from source to target, or an empty list if no path exists.
 */
std::vector<Vertex> get_path(const std::unordered_map<Vertex, std::optional<Vertex>>& predecessors,
                             const Vertex&                                              target) {
    std::vector<Vertex> path;
    std::optional<Vertex> current = target;
    while (current.has_value()) {
        path.push_back(*current);
        auto it = predecessors.find(*current);
        current = (it != predecessors.end()) ? it->second : std::nullopt;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// -------------------------------------------------------------------------
// Demos
// -------------------------------------------------------------------------

/**
 * Graph with a negative edge but no negative cycle.
 *
 *   A --5--> B
 *   A --2--> D
 *   B --5--> C
 *   C --10-> F
 *   D --5--> E
 *   E --7--> F
 *   E --(-10)--> B
 */
void demonstrate_normal_graph() {
    std::cout << "=== Example 1: Negative edge, no negative cycle ===\n";

    Vertex a("A"), b("B"), c("C"), d("D"), e("E"), f("F");
    std::vector<Vertex> vertices = {a, b, c, d, e, f};

    std::vector<Edge> edges = {
        {a, b,   5},
        {a, d,   2},
        {b, c,   5},
        {c, f,  10},
        {d, e,   5},
        {e, f,   7},
        {e, b, -10},
    };

    auto result = bellman_ford(vertices, edges, a);

    if (result.has_negative_cycle) {
        std::cout << "Negative cycle detected — distances are unreliable.\n";
    } else {
        std::cout << "Shortest distances and paths from source A:\n";
        for (const auto& v : vertices) {
            auto path = get_path(result.predecessors, v);
            std::cout << "  " << v.label() << ": distance=" << result.distances.at(v) << ", path=[";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << path[i].label();
                if (i + 1 < path.size()) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }
}

/**
 * Graph containing a negative weight cycle (B -> C -> F -> E -> B, total weight = -6).
 *
 *   A --5--> B
 *   A --2--> D
 *   B --3--> C
 *   C --4--> F
 *   D --5--> E
 *   F --2--> E
 *   E --(-15)--> B  <- closes the negative cycle
 */
void demonstrate_negative_cycle_graph() {
    std::cout << "=== Example 2: Graph with a negative cycle ===\n";

    Vertex a("A"), b("B"), c("C"), d("D"), e("E"), f("F");
    std::vector<Vertex> vertices = {a, b, c, d, e, f};

    std::vector<Edge> edges = {
        {a, b,   5},
        {a, d,   2},
        {b, c,   3},
        {c, f,   4},
        {d, e,   5},
        {f, e,   2},
        {e, b, -15},  // Creates negative cycle: B -> C -> F -> E -> B
    };

    auto result = bellman_ford(vertices, edges, a);

    if (result.has_negative_cycle) {
        std::cout << "Negative cycle detected — no finite shortest paths exist.\n";
    } else {
        std::cout << "No negative cycle found.\n";
    }
}

int main() {
    demonstrate_normal_graph();
    std::cout << "\n";
    demonstrate_negative_cycle_graph();
    return 0;
}
