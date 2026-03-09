#pragma once

#include <stdexcept>
#include "queue.h"

/**
 * A queue implementation using a singly linked list.
 * Grows dynamically; no fixed capacity.
 * front_ points to the head (dequeue end); rear_ points to the tail (enqueue end).
 */
class LinkedListQueue : public Queue {
public:
    ~LinkedListQueue() override {
        while (front_ != nullptr) {
            Node* temp = front_;
            front_     = front_->next;
            delete temp;
        }
    }

    bool is_empty() const override { return front_ == nullptr; }

    /**
     * Adds a character to the rear of the queue.
     */
    void enqueue(char value) override {
        Node* new_node = new Node(value);
        if (is_empty()) {
            front_ = new_node;
        } else {
            rear_->next = new_node;
        }
        rear_ = new_node;
    }

    char dequeue() override {
        if (is_empty()) throw std::runtime_error("Queue is empty");
        char  value = front_->data;
        Node* temp  = front_;
        front_      = front_->next;
        if (front_ == nullptr) rear_ = nullptr;
        delete temp;
        return value;
    }

    char peek() const override {
        if (is_empty()) throw std::runtime_error("Queue is empty");
        return front_->data;
    }

private:
    /**
     * Node in a linked list, holding a character and a reference to the next node.
     */
    struct Node {
        const char data;
        Node*      next = nullptr;

        explicit Node(char d) : data(d) {}
    };

    Node* front_ = nullptr;
    Node* rear_  = nullptr;
};
