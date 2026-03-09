#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "../../../graph_representation.h"
#include "../graph/prim_graph.h"
#include "../graph/prim_list_graph.h"
#include "../graph/prim_matrix_graph.h"

namespace graph {

/**
 * Creates a PrimGraph of the requested representation and pre-populates it with vertices.
 */
class GraphFactory {
public:
    GraphFactory() = delete;

    static std::unique_ptr<PrimGraph> create_graph(GraphRepresentation representation,
                                                    const std::vector<Vertex>& vertices) {
        std::unique_ptr<PrimGraph> graph;

        switch (representation) {
            case GraphRepresentation::MATRIX:
                graph = std::make_unique<PrimMatrixGraph>(static_cast<int>(vertices.size()));
                break;
            case GraphRepresentation::LIST:
                graph = std::make_unique<PrimListGraph>();
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
