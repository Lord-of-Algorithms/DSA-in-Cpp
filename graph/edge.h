#pragma once

#include <ostream>
#include "vertex.h"

namespace graph {

/**
 * Represents a directed edge in a graph, defined by a source vertex,
 * a destination vertex, and a weight.
 */
class Edge {
public:
    Edge(Vertex source, Vertex destination, int weight)
        : source_(std::move(source)),
          destination_(std::move(destination)),
          weight_(weight) {}

    const Vertex& source()      const { return source_; }
    const Vertex& destination() const { return destination_; }
    int           weight()      const { return weight_; }

    friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
        return os << "[" << e.source_ << "->" << e.destination_ << ":" << e.weight_ << "]";
    }

private:
    Vertex source_;
    Vertex destination_;
    int    weight_;
};

} // namespace graph
