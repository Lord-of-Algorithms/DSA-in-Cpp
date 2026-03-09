#pragma once

#include <stdexcept>
#include <vector>
#include "../vertex.h"

namespace graph {

/**
 * A simple priority queue for Dijkstra's algorithm that manages vertices by their distance
 * from the source. Implemented as a sorted vector for educational clarity.
 *
 * Entries are kept in descending order of distance so that the vertex with the smallest
 * distance always sits at the back and can be removed in O(1). Insertion and update
 * are O(n) — intentionally straightforward rather than optimised.
 */
class VertexDistancePriorityQueue {
public:
    explicit VertexDistancePriorityQueue(int max_size) : max_size_(max_size) {
        if (max_size <= 0) {
            throw std::invalid_argument("Maximum size must be greater than 0.");
        }
        entries_.reserve(max_size);
    }

    bool is_empty() const { return entries_.empty(); }
    bool is_full()  const { return static_cast<int>(entries_.size()) == max_size_; }

    /**
     * Adds a vertex with its distance, inserting it in sorted position (descending by distance).
     * @throws std::overflow_error if the queue is full.
     */
    void add(const Vertex& vertex, int distance) {
        if (is_full()) {
            throw std::overflow_error("Queue is full.");
        }
        // Find insertion point: maintain descending order (largest distance at front)
        auto it = entries_.begin();
        while (it != entries_.end() && it->distance > distance) {
            ++it;
        }
        entries_.insert(it, {vertex, distance});
    }

    /**
     * Removes and returns the vertex with the smallest distance (last element).
     * @throws std::underflow_error if the queue is empty.
     */
    Vertex poll_smallest() {
        if (is_empty()) {
            throw std::underflow_error("Queue is empty.");
        }
        Vertex v = entries_.back().vertex;
        entries_.pop_back();
        return v;
    }

    /**
     * Updates the distance of an existing vertex, re-inserting it at the correct position.
     * @throws std::out_of_range if the vertex is not found.
     */
    void update(const Vertex& vertex, int new_distance) {
        auto it = entries_.end();
        for (auto i = entries_.begin(); i != entries_.end(); ++i) {
            if (i->vertex == vertex) {
                it = i;
                break;
            }
        }
        if (it == entries_.end()) {
            throw std::out_of_range("Vertex not found in queue.");
        }
        entries_.erase(it);
        add(vertex, new_distance);
    }

private:
    struct Entry {
        Vertex vertex;
        int    distance;
    };

    int              max_size_;
    std::vector<Entry> entries_;
};

} // namespace graph
