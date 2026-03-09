#pragma once

#include <ostream>
#include <string>
#include <utility>

namespace graph {

/**
 * Represents a vertex in a graph, identified by a string label.
 */
class Vertex {
public:
    explicit Vertex(std::string label) : label_(std::move(label)) {}

    const std::string& label() const { return label_; }

    bool operator==(const Vertex& other) const { return label_ == other.label_; }
    bool operator!=(const Vertex& other) const { return !(*this == other); }
    bool operator< (const Vertex& other) const { return label_ <  other.label_; }

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        return os << v.label_;
    }

private:
    std::string label_;
};

} // namespace graph


// Hash support so Vertex can be used as an unordered_map key
namespace std {
template <>
struct hash<graph::Vertex> {
    size_t operator()(const graph::Vertex& v) const {
        return hash<string>{}(v.label());
    }
};
} // namespace std
