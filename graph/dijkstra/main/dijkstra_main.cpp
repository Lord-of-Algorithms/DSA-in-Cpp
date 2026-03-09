/**
 * Demonstrates Dijkstra's algorithm to find shortest paths in a graph,
 * showing how the shortest path to D changes as edge weights are updated.
 */

#include <iostream>
#include <vector>
#include "graph_factory.h"
#include "../dijkstra_algorithm.h"
#include "../../graph_representation.h"
#include "../../vertex.h"

using namespace graph;

void print_path(const std::vector<Vertex>& path) {
    std::cout << "[";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i].label();
        if (i + 1 < path.size()) std::cout << ", ";
    }
    std::cout << "]";
}

int main() {
    Vertex a("A"), b("B"), c("C"), d("D");
    std::vector<Vertex> vertices = {a, b, c, d};

    auto graph = create_graph(GraphRepresentation::LIST, vertices);

    graph->set_edge(a, b, 13);
    graph->set_edge(c, b, 18);
    graph->set_edge(a, c, 13);
    graph->set_edge(b, d, 28);
    graph->set_edge(c, d, 85);

    DijkstraAlgorithm dijkstra;
    dijkstra.compute_paths(*graph, a);
    std::cout << "Initial path to D: ";
    print_path(dijkstra.get_shortest_path_to(d));
    std::cout << "\n";

    // Update edges and recompute paths
    dijkstra.reset_state();
    graph->set_edge(a, d, 10); // Direct path added
    dijkstra.compute_paths(*graph, a);
    std::cout << "Updated path to D with direct A to D: ";
    print_path(dijkstra.get_shortest_path_to(d));
    std::cout << "\n";

    dijkstra.reset_state();
    graph->set_edge(a, b, 53);  // Increased weight A to B
    graph->set_edge(a, d, 100); // Less optimal direct path A to D
    dijkstra.compute_paths(*graph, a);
    std::cout << "Path after changing weights: ";
    print_path(dijkstra.get_shortest_path_to(d));
    std::cout << "\n";

    return 0;
}
