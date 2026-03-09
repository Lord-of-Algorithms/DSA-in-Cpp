#pragma once

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <vector>

#include "../../edge.h"
#include "../../vertex.h"

namespace graph {

/**
 * A priority queue for storing and retrieving edges based on their weights,
 * where the edge with the smallest weight is always prioritized for retrieval.
 *
 * Internally kept in descending order so the smallest-weight edge sits at the
 * back and can be retrieved in O(1). Insertion and replacement are O(n).
 */
class EdgePriorityQueue {
public:
    /**
     * Constructs a priority queue with a specified maximum capacity.
     */
    explicit EdgePriorityQueue(int max_size) {
        if (max_size <= 0) {
            throw std::invalid_argument("Maximum size must be greater than 0.");
        }
        entries_.reserve(max_size);
        max_size_ = max_size;
    }

    bool is_empty() const { return entries_.empty(); }
    bool is_full()  const { return static_cast<int>(entries_.size()) == max_size_; }

    /**
     * Adds an edge in a position that maintains descending weight order.
     * Performed in O(n) time.
     */
    void add(const Edge& edge) {
        if (is_full()) throw std::overflow_error("Queue is full.");
        // Find first position whose weight is <= new edge weight and insert before it,
        // keeping the vector in descending order (smallest weight at back).
        auto it = entries_.begin();
        while (it != entries_.end() && it->weight() > edge.weight()) ++it;
        entries_.insert(it, edge);
    }

    /**
     * Retrieves, but does not remove, the smallest weight edge.
     * Returns nullopt if the queue is empty.
     */
    std::optional<Edge> peek_smallest() const {
        if (is_empty()) return std::nullopt;
        return entries_.back();
    }

    /**
     * Retrieves and removes the smallest weight edge.
     */
    Edge poll_smallest() {
        if (is_empty()) throw std::underflow_error("Queue is empty.");
        Edge e = entries_.back();
        entries_.pop_back();
        return e;
    }

    /**
     * Replaces one edge with another while maintaining the queue's order.
     * The target edge is identified by its destination vertex.
     * Performed in O(n) time.
     */
    void replace(const Edge& target, const Edge& replacement) {
        auto it = std::find_if(entries_.begin(), entries_.end(),
            [&](const Edge& e) { return e.destination() == target.destination(); });
        if (it == entries_.end()) {
            throw std::runtime_error("Target edge not found in queue.");
        }
        entries_.erase(it);
        add(replacement);
    }

    /**
     * Finds an edge with a specific destination vertex.
     * Returns nullopt if no such edge exists.
     * Performed in O(n) time.
     */
    std::optional<Edge> find_edge_with_destination(const Vertex& destination) const {
        for (const auto& e : entries_) {
            if (e.destination() == destination) return e;
        }
        return std::nullopt;
    }

private:
    std::vector<Edge> entries_;
    int max_size_;
};

} // namespace graph
