#pragma once

#include <stdexcept>
#include "stack.h"

/**
 * Implementation of the Stack interface using a static-sized array.
 */
class StaticArrayStack : public Stack {
public:
    /**
     * Constructor to initialize the stack with a given capacity.
     *
     * @param capacity The maximum number of items the stack can hold.
     * @throws std::invalid_argument if capacity < 1.
     */
    explicit StaticArrayStack(int capacity) {
        if (capacity < 1) {
            throw std::invalid_argument("Capacity must be at least 1");
        }
        data_ = new char[capacity];
        top_  = -1;
        cap_  = capacity;
    }

    ~StaticArrayStack() override { delete[] data_; }

    bool is_empty() const override { return top_ == -1; }

    /**
     * Adds a character to the top of the stack.
     *
     * @throws std::overflow_error if the stack is full.
     */
    void push(char value) override {
        if (is_full()) {
            throw std::overflow_error("Stack is full");
        }
        data_[++top_] = value;
    }

    char pop() override {
        if (is_empty()) throw std::runtime_error("Stack is empty");
        return data_[top_--];
    }

    char peek() const override {
        if (is_empty()) throw std::runtime_error("Stack is empty");
        return data_[top_];
    }

private:
    char* data_;
    int   top_;
    int   cap_;

    bool is_full() const { return top_ == cap_ - 1; }
};
