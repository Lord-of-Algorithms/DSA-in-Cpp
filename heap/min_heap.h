#pragma once

#include <stdexcept>
#include <string>
#include <vector>

/**
 * A binary min-heap backed by a fixed-capacity array.
 *
 * A heap is a complete binary tree — every level is full except possibly
 * the last, which fills left to right — that satisfies the heap property:
 * every parent is less than or equal to its children. As a result the
 * smallest value is always at the root.
 *
 * Because the tree is complete it maps directly onto an array with no
 * gaps, so no node objects or pointers are needed. For the node at index
 * i:
 *
 *   * its parent is at (i - 1) / 2,
 *   * its left child at 2 * i + 1,
 *   * its right child at 2 * i + 2.
 *
 * All operations run in place. insert and extract_min are O(log n);
 * peek_min is O(1).
 */
class MinHeap {
public:
    /**
     * Creates an empty heap that can hold up to capacity values.
     * Throws std::invalid_argument if capacity is not positive.
     */
    explicit MinHeap(int capacity) : size_(0) {
        if (capacity <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }
        heap_.resize(capacity);
    }

    // Returns true if the heap holds no values.
    bool is_empty() const { return size_ == 0; }

    // Returns true if the heap has no room for another value.
    bool is_full() const {
        return size_ == static_cast<int>(heap_.size());
    }

    // Returns the number of values currently in the heap.
    int size() const { return size_; }

    // Returns the smallest value without removing it.
    // Throws std::out_of_range if the heap is empty.
    int peek_min() const {
        if (is_empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap_[0];
    }

    // Inserts a value at the first free slot, then sifts it up until the
    // heap property is restored. Throws std::overflow_error if full.
    void insert(int value) {
        if (is_full()) {
            throw std::overflow_error("Heap is full");
        }
        heap_[size_] = value;
        sift_up(size_);
        size_++;
    }

    // Removes and returns the smallest value. The root is swapped with the
    // last leaf, that leaf is dropped, and the new root is sifted down.
    // Throws std::out_of_range if the heap is empty.
    int extract_min() {
        if (is_empty()) {
            throw std::out_of_range("Heap is empty");
        }
        swap(0, size_ - 1);
        int minimum = heap_[size_ - 1];
        size_--;
        if (!is_empty()) {
            sift_down(0);
        }
        return minimum;
    }

    // Removes the node at index i. The last leaf fills the gap and is then
    // sifted up or down until the heap property is restored.
    // (`delete` is a C++ keyword, so this operation is named `remove`.)
    // Throws std::out_of_range if i is not a valid index.
    void remove(int i) {
        if (i < 0 || i >= size_) {
            throw std::out_of_range(
                "No node at index " + std::to_string(i));
        }
        swap(i, size_ - 1);
        size_--;
        if (i == size_) {
            return;  // removed the last leaf
        }
        if (i > 0 && heap_[i] < heap_[parent(i)]) {
            sift_up(i);
        } else {
            sift_down(i);
        }
    }

private:
    std::vector<int> heap_;
    int size_;

    static int parent(int i) { return (i - 1) / 2; }
    static int left_child(int i) { return 2 * i + 1; }
    static int right_child(int i) { return 2 * i + 2; }

    void swap(int i, int j) {
        int temp = heap_[i];
        heap_[i] = heap_[j];
        heap_[j] = temp;
    }

    // Moves node i up while it is smaller than its parent.
    void sift_up(int i) {
        while (i > 0 && heap_[i] < heap_[parent(i)]) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    // Moves node i down while it is larger than its smaller child. A node
    // with no left child is a leaf, so the sift stops there.
    void sift_down(int i) {
        while (left_child(i) < size_) {
            int smaller = left_child(i);
            int right = right_child(i);
            if (right < size_ && heap_[right] < heap_[smaller]) {
                smaller = right;
            }
            if (heap_[i] <= heap_[smaller]) {
                break;
            }
            swap(i, smaller);
            i = smaller;
        }
    }
};
