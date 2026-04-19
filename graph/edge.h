#pragma once

#include <ostream>
#include "vertex.h"

namespace graph {

/**
 * Represents an unweighted directed edge between two vertices.
 */
class Edge {
public:
    Edge(Vertex source, Vertex destination)
        : source_(std::move(source)),
          destination_(std::move(destination)) {}

    const Vertex& source()      const { return source_; }
    const Vertex& destination() const { return destination_; }

    friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
        return os << e.source_ << "->" << e.destination_;
    }

private:
    Vertex source_;
    Vertex destination_;
};

} // namespace graph
