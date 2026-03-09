#pragma once

#include <algorithm>
#include <stdexcept>
#include "queue.h"

/**
 * A queue that automatically adjusts its capacity based on the number of elements.
 * Uses a circular array internally.
 * Doubles capacity when full; halves when 1/4 full (minimum capacity 10).
 */
class DynamicArrayQueue : public Queue {
public:
    /**
     * Constructs a dynamic queue with a specified initial capacity.
     *
     * @param capacity Initial capacity. Must be >= 1.
     * @throws std::invalid_argument if capacity < 1.
     */
    explicit DynamicArrayQueue(int capacity) {
        if (capacity < 1) {
            throw std::invalid_argument("Capacity must be at least 1");
        }
        data_  = new char[capacity];
        cap_   = capacity;
        front_ = 0;
        rear_  = -1;
        size_  = 0;
    }

    ~DynamicArrayQueue() override { delete[] data_; }

    bool is_empty() const override { return size_ == 0; }

    /**
     * Adds a character to the rear of the queue. Resizes if full.
     */
    void enqueue(char value) override {
        if (size_ == cap_) {
            resize(2 * cap_);  // Double the size when full
        }
        rear_        = (rear_ + 1) % cap_;
        data_[rear_] = value;
        ++size_;
    }

    char dequeue() override {
        if (is_empty()) throw std::runtime_error("Queue is empty");

        char value = data_[front_];
        front_     = (front_ + 1) % cap_;
        --size_;

        // Reduce size when queue is 1/4 full (and at least one element remains)
        if (size_ > 0 && size_ == cap_ / 4) {
            resize(std::max(cap_ / 2, 10));  // Ensure capacity doesn't get too small
        }

        return value;
    }

    char peek() const override {
        if (is_empty()) throw std::runtime_error("Queue is empty");
        return data_[front_];
    }

private:
    char* data_;
    int   cap_;
    int   front_;
    int   rear_;
    int   size_;

    /**
     * Resizes the underlying circular array, copying elements in logical order.
     */
    void resize(int new_capacity) {
        char* new_data = new char[new_capacity];
        for (int i = 0; i < size_; ++i) {
            new_data[i] = data_[(front_ + i) % cap_];
        }
        delete[] data_;
        data_  = new_data;
        cap_   = new_capacity;
        front_ = 0;
        rear_  = size_ - 1;
    }
};
