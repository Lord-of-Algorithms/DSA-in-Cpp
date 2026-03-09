#pragma once

#include <stdexcept>
#include "stack.h"

/**
 * Implements the Stack interface using a singly linked list.
 * Uses a private Node class; grows dynamically without a fixed capacity.
 */
class LinkedListStack : public Stack {
public:
    ~LinkedListStack() override {
        while (top_ != nullptr) {
            Node* temp = top_;
            top_       = top_->next;
            delete temp;
        }
    }

    bool is_empty() const override { return top_ == nullptr; }

    void push(char value) override {
        Node* node = new Node(value);
        node->next = top_;
        top_       = node;
    }

    char pop() override {
        if (is_empty()) throw std::runtime_error("Stack is empty");
        char value = top_->data;
        Node* temp = top_;
        top_       = top_->next;
        delete temp;
        return value;
    }

    char peek() const override {
        if (is_empty()) throw std::runtime_error("Stack is empty");
        return top_->data;
    }

private:
    /**
     * Node represents an element in the stack,
     * containing a value and reference to the next Node.
     */
    struct Node {
        const char data;
        Node*      next = nullptr;

        explicit Node(char d) : data(d) {}
    };

    Node* top_ = nullptr;
};
