#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include "../../graph_representation.h"
#include "../graph/dijkstra_graph.h"
#include "../graph/dijkstra_list_graph.h"
#include "../graph/dijkstra_matrix_graph.h"

namespace graph {

/**
 * Creates a DijkstraGraph of the requested representation, pre-populated with the given vertices.
 */
inline std::unique_ptr<DijkstraGraph> create_graph(GraphRepresentation representation,
                                                   const std::vector<Vertex>& vertices) {
    std::unique_ptr<DijkstraGraph> graph;

    switch (representation) {
        case GraphRepresentation::LIST:
            graph = std::make_unique<DijkstraListGraph>();
            break;
        case GraphRepresentation::MATRIX:
            graph = std::make_unique<DijkstraMatrixGraph>(static_cast<int>(vertices.size()));
            break;
        default:
            throw std::invalid_argument("Unknown graph representation.");
    }

    for (const auto& v : vertices) {
        graph->add_vertex(v);
    }
    return graph;
}

} // namespace graph
