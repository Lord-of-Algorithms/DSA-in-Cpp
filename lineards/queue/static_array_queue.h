#pragma once

#include <stdexcept>
#include "queue.h"

/**
 * A fixed-size queue implemented using a circular array.
 * Enqueue and dequeue are both O(1); no shifting of elements is needed.
 */
class StaticArrayQueue : public Queue {
public:
    /**
     * Constructs a new queue with a specified capacity.
     *
     * @param capacity The maximum number of items the queue can hold.
     * @throws std::invalid_argument if capacity < 1.
     */
    explicit StaticArrayQueue(int capacity) {
        if (capacity < 1) {
            throw std::invalid_argument("Capacity must be at least 1");
        }
        data_  = new char[capacity];
        cap_   = capacity;
        front_ = 0;
        rear_  = -1;
        size_  = 0;
    }

    ~StaticArrayQueue() override { delete[] data_; }

    bool is_empty() const override { return size_ == 0; }

    /**
     * Checks if the queue is full.
     */
    bool is_full() const { return size_ == cap_; }

    /**
     * Adds a character to the rear of the queue.
     *
     * @throws std::overflow_error if the queue is full.
     */
    void enqueue(char value) override {
        if (is_full()) throw std::overflow_error("Queue is full");
        rear_        = (rear_ + 1) % cap_;
        data_[rear_] = value;
        ++size_;
    }

    char dequeue() override {
        if (is_empty()) throw std::runtime_error("Queue is empty");
        char value = data_[front_];
        front_     = (front_ + 1) % cap_;
        --size_;
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
};
