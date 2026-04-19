#pragma once

#include <ostream>
#include "edge.h"

namespace graph {

/**
 * Represents a weighted directed edge between two vertices.
 */
class WeightedEdge : public Edge {
public:
    WeightedEdge(Vertex source, Vertex destination, int weight)
        : Edge(std::move(source), std::move(destination)),
          weight_(weight) {}

    int weight() const { return weight_; }

    friend std::ostream& operator<<(std::ostream& os, const WeightedEdge& e) {
        return os << "[" << e.source() << "->" << e.destination() << ":" << e.weight_ << "]";
    }

private:
    int weight_;
};

} // namespace graph
