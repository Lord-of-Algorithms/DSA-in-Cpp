/**
 * Demonstrates Kahn's algorithm for computing a topological ordering of a
 * directed acyclic graph (DAG).
 *
 * A topological ordering is a linear sequence of all vertices such that for
 * every directed edge u -> v, vertex u appears before vertex v in the sequence.
 *
 * Kahn's algorithm uses in-degrees — the number of incoming edges per vertex —
 * to determine which vertices are ready to be processed at each step.
 *
 * Time complexity: O(V + E), where V is the number of vertices and E is the number of edges.
 */

#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../vertex.h"
#include "../edge.h"

using namespace graph;

// -------------------------------------------------------------------------
// Result holder
// -------------------------------------------------------------------------

/**
 * Holds the output of the topological sort: the ordering of vertices and a
 * flag indicating whether a cycle was detected.
 */
struct TopologicalSortResult {
    std::vector<Vertex> order;
    bool                has_cycle;
};

// -------------------------------------------------------------------------
// Algorithm
// -------------------------------------------------------------------------

/**
 * Computes a topological ordering of the given DAG using Kahn's algorithm.
 *
 * @param vertices  All vertices in the graph.
 * @param edges     All directed edges in the graph.
 * @return A TopologicalSortResult containing the vertex order and a cycle flag.
 */
static TopologicalSortResult topological_sort(const std::vector<Vertex>& vertices,
                                               const std::vector<Edge>&   edges) {
    if (vertices.empty()) {
        throw std::invalid_argument("Vertex list cannot be empty.");
    }

    // Step 1: Compute in-degrees and build adjacency list from the edge list
    std::unordered_map<Vertex, int>                in_degree;
    std::unordered_map<Vertex, std::vector<Vertex>> adjacency;

    for (const auto& v : vertices) {
        in_degree[v] = 0;
        adjacency[v] = {};
    }

    for (const auto& edge : edges) {
        const Vertex& source      = edge.source();
        const Vertex& destination = edge.destination();

        adjacency[source].push_back(destination);
        in_degree[destination] = in_degree[destination] + 1;
    }

    // Step 2: Enqueue all vertices with in-degree 0 — they have no dependencies
    std::queue<Vertex> queue;
    for (const auto& v : vertices) {
        if (in_degree[v] == 0) {
            queue.push(v);
        }
    }

    // Step 3: Process the queue
    std::vector<Vertex> result;
    while (!queue.empty()) {
        Vertex vertex = queue.front();
        queue.pop();
        result.push_back(vertex);

        for (const Vertex& neighbour : adjacency[vertex]) {
            in_degree[neighbour] = in_degree[neighbour] - 1;
            if (in_degree[neighbour] == 0) {
                queue.push(neighbour);
            }
        }
    }

    // Cycle detection: if not all vertices were processed, a cycle exists
    bool has_cycle = static_cast<int>(result.size()) != static_cast<int>(vertices.size());
    return {result, has_cycle};
}

// -------------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------------

static void print_result(const TopologicalSortResult& result) {
    if (result.has_cycle) {
        std::cout << "Cycle detected — topological sort is not possible.\n";
    } else {
        std::cout << "Topological order: [";
        for (size_t i = 0; i < result.order.size(); ++i) {
            std::cout << result.order[i];
            if (i + 1 < result.order.size()) std::cout << ", ";
        }
        std::cout << "]\n";
    }
}

// -------------------------------------------------------------------------
// Demos
// -------------------------------------------------------------------------

/**
 * A directed acyclic graph representing a simple dependency structure.
 *
 *     A --> B
 *     A --> C
 *     B --> D
 *     C --> D
 *     D --> E
 */
static void demonstrate_dag() {
    std::cout << "=== Example 1: Directed acyclic graph ===\n";

    Vertex a("A"), b("B"), c("C"), d("D"), e("E");
    std::vector<Vertex> vertices = {a, b, c, d, e};

    std::vector<Edge> edges = {
        {a, b},
        {a, c},
        {b, d},
        {c, d},
        {d, e},
    };

    auto result = topological_sort(vertices, edges);
    print_result(result);
}

/**
 * A graph containing a cycle (B -> C -> D -> B), making topological sort impossible.
 *
 *     A --> B
 *     B --> C
 *     C --> D
 *     D --> B  <- closes the cycle
 */
static void demonstrate_cyclic_graph() {
    std::cout << "=== Example 2: Graph with a cycle ===\n";

    Vertex a("A"), b("B"), c("C"), d("D");
    std::vector<Vertex> vertices = {a, b, c, d};

    std::vector<Edge> edges = {
        {a, b},
        {b, c},
        {c, d},
        {d, b},  // Creates cycle: B -> C -> D -> B
    };

    auto result = topological_sort(vertices, edges);
    print_result(result);
}

int main() {
    demonstrate_dag();
    std::cout << "\n";
    demonstrate_cyclic_graph();
    return 0;
}
