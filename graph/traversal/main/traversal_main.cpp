/**
 * Demonstrates graph creation, edge setup, and traversal using Depth-First Search (DFS)
 * and Breadth-First Search (BFS) algorithms.
 *
 * BFS explores the graph level by level (all neighbours first), while DFS goes as deep
 * as possible along each branch before backtracking. Both use the same ExplorableGraph
 * interface, demonstrating that the traversal algorithm is decoupled from the graph structure.
 */

#include <iostream>
#include <memory>
#include <vector>

#include "../../graph_representation.h"
#include "../../vertex.h"
#include "../algorithms/graph_traversal.h"
#include "../graph/explorable_graph.h"
#include "graph_factory.h"
#include "graph_traversal_factory.h"
#include "traversal_method.h"

using namespace graph;

static void print_path(const GraphTraversal& traversal) {
    std::cout << traversal.name() << ": [";
    const auto& path = traversal.get_traversal_path();
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i + 1 < path.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    Vertex a("A"), b("B"), c("C"), d("D");
    std::vector<Vertex> vertices = {a, b, c, d};

    std::unique_ptr<ExplorableGraph> graph =
        GraphFactory::create_graph(GraphRepresentation::LIST, vertices);

    graph->set_edge(a, b);
    graph->set_edge(a, d);
    graph->set_edge(b, c);
    graph->set_edge(b, d);
    graph->set_edge(c, d);

    // Depth-First Search
    std::unique_ptr<GraphTraversal> traversal =
        GraphTraversalFactory::create_traversal(TraversalMethod::DFS);
    traversal->traverse(*graph, a);
    print_path(*traversal);
    traversal->reset_state();

    // Breadth-First Search
    traversal = GraphTraversalFactory::create_traversal(TraversalMethod::BFS);
    traversal->traverse(*graph, a);
    print_path(*traversal);

    return 0;
}
