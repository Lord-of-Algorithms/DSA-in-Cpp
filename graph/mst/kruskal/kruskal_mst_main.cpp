/**
 * Demonstrates the use of Kruskal's algorithm to find the minimum spanning tree (MST) of a graph.
 *
 * Kruskal's algorithm is particularly suited for graphs represented as a list of edges, as it
 * processes edges in order of increasing weight regardless of their position in the graph.
 * This edge-list representation is used because Kruskal's algorithm does not require direct
 * access to the graph's adjacency structure, making it efficient and straightforward for
 * calculating the MST in graphs where edge connectivity is the primary concern.
 *
 * Time complexity: O(E log E) for sorting edges, where E is the number of edges.
 * Union-find operations add near-constant overhead per edge with path compression and union by rank.
 */

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../../vertex.h"
#include "../../edge.h"
#include "union_find.h"

using namespace graph;

/**
 * Constructs the minimum spanning tree (MST) for a graph represented by vertices and edges.
 * Assumes that the graph is connected.
 */
static std::vector<Edge> build_mst(const std::vector<Vertex>& vertices, std::vector<Edge> edges) {
    if (vertices.empty()) {
        throw std::invalid_argument("Vertex list cannot be empty.");
    }
    if (edges.empty()) {
        throw std::invalid_argument("Edge list cannot be empty.");
    }

    std::vector<Edge> mst;

    // Sort edges by weight in ascending order
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight() < b.weight();
    });

    UnionFind uf(vertices);

    for (const Edge& edge : edges) {
        if (uf.find(edge.source()) != uf.find(edge.destination())) {
            mst.push_back(edge);
            uf.union_sets(edge.source(), edge.destination());
            if (static_cast<int>(mst.size()) == static_cast<int>(vertices.size()) - 1) {
                // MST is complete — no need to examine remaining edges
                break;
            }
        }
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

    std::vector<Edge> edges = {
        {a, b, 1},
        {d, b, 2},
        {b, c, 3},
        {a, d, 4},
        {d, c, 5},
    };

    std::vector<Edge> mst = build_mst(vertices, edges);
    print_mst(mst);

    return 0;
}
