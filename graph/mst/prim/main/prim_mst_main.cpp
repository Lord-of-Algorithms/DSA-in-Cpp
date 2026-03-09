/**
 * Demonstrates the use of Prim's algorithm to find the minimum spanning tree (MST) of a graph.
 *
 * Unlike Kruskal's algorithm which sorts all edges upfront, Prim's algorithm grows the MST
 * one vertex at a time from a starting vertex. At each step it picks the cheapest edge that
 * connects a vertex already in the tree to a vertex not yet in the tree.
 *
 * This implementation uses a custom EdgePriorityQueue (sorted vector) to always retrieve the
 * minimum-weight candidate edge in O(1), while insertion and replacement are O(n).
 * For large graphs a binary heap would give O(log n) insertion, but the sorted-vector approach
 * matches the Java reference implementation and keeps the code easy to follow.
 *
 * Time complexity: O(V^2) with this sorted-vector priority queue, where V is the number of vertices.
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "../../../graph_representation.h"
#include "../../../vertex.h"
#include "../../../edge.h"
#include "../edge_priority_queue.h"
#include "../graph/prim_graph.h"
#include "graph_factory.h"

using namespace graph;

/**
 * Constructs the minimum spanning tree (MST) for the given graph starting from startVertex.
 * Implements Prim's algorithm: incrementally builds the MST by always selecting the
 * smallest-weight edge that connects the current tree to an unvisited vertex.
 */
static std::vector<Edge> build_mst(PrimGraph& graph, const Vertex& start_vertex) {
    if (!graph.contains_vertex(start_vertex)) {
        throw std::invalid_argument("Start vertex is not part of the graph.");
    }

    int vertex_count = graph.get_vertex_count();
    if (vertex_count < 1) {
        throw std::invalid_argument("Graph must contain at least one vertex.");
    }

    std::unordered_set<Vertex> in_tree;
    std::vector<Edge> mst;
    EdgePriorityQueue pqueue(vertex_count);

    Vertex current = start_vertex;

    while (static_cast<int>(in_tree.size()) < vertex_count - 1) {
        in_tree.insert(current);

        for (const Edge& edge : graph.get_edges_for_source(current)) {
            const Vertex& neighbour = edge.destination();
            if (!in_tree.count(neighbour)) {
                std::optional<Edge> existing = pqueue.find_edge_with_destination(neighbour);
                if (existing.has_value()) {
                    if (existing->weight() > edge.weight()) {
                        pqueue.replace(*existing, edge);
                    }
                } else {
                    pqueue.add(edge);
                }
            }
        }

        if (pqueue.is_empty()) {
            throw std::runtime_error("Graph is disconnected — MST cannot be completed.");
        }

        Edge smallest = pqueue.poll_smallest();
        current = smallest.destination();
        mst.push_back(smallest);
    }

    return mst;
}

static void print_mst(const std::vector<Edge>& mst) {
    std::cout << "MST: [";
    for (size_t i = 0; i < mst.size(); ++i) {
        std::cout << mst[i];
        if (i + 1 < mst.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    Vertex a("A"), b("B"), c("C"), d("D");
    std::vector<Vertex> vertices = {a, b, c, d};

    std::unique_ptr<PrimGraph> graph =
        GraphFactory::create_graph(GraphRepresentation::MATRIX, vertices);

    graph->set_edge(a, b, 1);
    graph->set_edge(d, b, 2);
    graph->set_edge(b, c, 3);
    graph->set_edge(a, d, 4);
    graph->set_edge(d, c, 5);

    print_mst(build_mst(*graph, a));

    // Modify an edge weight and recompute
    graph->set_edge(a, b, 6);
    std::cout << "MST after changing weight: ";
    print_mst(build_mst(*graph, a));

    return 0;
}
