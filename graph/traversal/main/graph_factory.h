#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "../../graph_representation.h"
#include "../graph/explorable_graph.h"
#include "../graph/explorable_list_graph.h"
#include "../graph/explorable_matrix_graph.h"

namespace graph {

/**
 * Creates an ExplorableGraph of the requested representation and pre-populates it with vertices.
 */
class GraphFactory {
public:
    GraphFactory() = delete;

    static std::unique_ptr<ExplorableGraph> create_graph(GraphRepresentation representation,
                                                          const std::vector<Vertex>& vertices) {
        std::unique_ptr<ExplorableGraph> graph;

        switch (representation) {
            case GraphRepresentation::MATRIX:
                graph = std::make_unique<ExplorableMatrixGraph>(static_cast<int>(vertices.size()));
                break;
            case GraphRepresentation::LIST:
                graph = std::make_unique<ExplorableListGraph>();
                break;
            default:
                throw std::invalid_argument("Unknown graph representation.");
        }

        for (const auto& v : vertices) {
            graph->add_vertex(v);
        }
        return graph;
    }
};

} // namespace graph
