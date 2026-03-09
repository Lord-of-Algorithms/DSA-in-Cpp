#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>

/**
 * A custom dynamic array implementation that automatically resizes itself as needed
 * while providing high-performance array operations.
 * Uses a raw int array internally, doubling capacity on full and halving when ≤ 1/4 full.
 */
class DynamicArray {
public:
    /**
     * Constructs a dynamic array with a specified initial capacity.
     *
     * @param capacity The initial capacity. Must be >= 1.
     * @throws std::invalid_argument if capacity < 1.
     */
    explicit DynamicArray(int capacity) {
        if (capacity < 1) {
            throw std::invalid_argument("Capacity must be at least 1");
        }
        array_    = new int[capacity]{};
        size_     = 0;
        capacity_ = capacity;
    }

    ~DynamicArray() { delete[] array_; }

    /**
     * Returns the value at the given index.
     */
    int get(int index) const {
        check_bounds(index);
        return array_[index];
    }

    /**
     * Sets the value at the given index.
     */
    void set(int index, int value) {
        check_bounds(index);
        array_[index] = value;
    }

    /**
     * Adds a new value to the end of this dynamic array.
     */
    void add(int value) {
        ensure_capacity();
        array_[size_++] = value;
    }

    /**
     * Inserts a new value at the specified index.
     */
    void add(int index, int value) {
        check_bounds(index);
        ensure_capacity();
        // Shift elements from index onwards one position to the right
        for (int i = size_; i > index; --i) {
            array_[i] = array_[i - 1];
        }
        array_[index] = value;
        ++size_;
    }

    /**
     * Removes the value at the given index.
     */
    void remove(int index) {
        check_bounds(index);
        // Shift elements after index one position to the left
        for (int i = index; i < size_ - 1; ++i) {
            array_[i] = array_[i + 1];
        }
        --size_;
        shrink_capacity();
    }

    int size()     const { return size_; }
    int capacity() const { return capacity_; }

private:
    int* array_;
    int  size_;
    int  capacity_;

    void check_bounds(int index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range(
                "Index: " + std::to_string(index) + ", Size: " + std::to_string(size_));
        }
    }

    void ensure_capacity() {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
    }

    void shrink_capacity() {
        if (size_ <= capacity_ / 4 && capacity_ > 1) {
            resize(std::max(capacity_ / 2, 1));
        }
    }

    void resize(int new_capacity) {
        int* new_array = new int[new_capacity]{};
        for (int i = 0; i < size_; ++i) {
            new_array[i] = array_[i];
        }
        delete[] array_;
        array_    = new_array;
        capacity_ = new_capacity;
    }
};
