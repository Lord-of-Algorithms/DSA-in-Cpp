#pragma once

#include <stdexcept>
#include "stack.h"

/**
 * A dynamic array implementation of a stack that resizes as elements are added or removed.
 * Doubles capacity when full; halves when 1/4 full (with at least one element remaining).
 */
class DynamicArrayStack : public Stack {
public:
    /**
     * Constructor to initialize the stack with an initial capacity.
     *
     * @param capacity Initial capacity. Must be >= 1.
     * @throws std::invalid_argument if capacity < 1.
     */
    explicit DynamicArrayStack(int capacity) {
        if (capacity < 1) {
            throw std::invalid_argument("Initial capacity must be at least 1");
        }
        data_ = new char[capacity];
        top_  = -1;
        cap_  = capacity;
    }

    ~DynamicArrayStack() override { delete[] data_; }

    bool is_empty() const override { return top_ == -1; }

    void push(char value) override {
        if (top_ + 1 == cap_) {
            resize(cap_ * 2);  // Double the size when full
        }
        data_[++top_] = value;
    }

    char pop() override {
        if (is_empty()) throw std::runtime_error("Stack is empty");

        char value = data_[top_--];

        // Reduce size when less than 1/4 full (and at least one element remains)
        int count = top_ + 1;
        if (count > 0 && count == cap_ / 4) {
            resize(cap_ / 2);
        }

        return value;
    }

    char peek() const override {
        if (is_empty()) throw std::runtime_error("Stack is empty");
        return data_[top_];
    }

private:
    char* data_;
    int   top_;
    int   cap_;

    void resize(int new_capacity) {
        char* new_array = new char[new_capacity];
        for (int i = 0; i <= top_; ++i) {
            new_array[i] = data_[i];
        }
        delete[] data_;
        data_ = new_array;
        cap_  = new_capacity;
    }
};
