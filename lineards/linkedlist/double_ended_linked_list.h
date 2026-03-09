#pragma once

#include <stdexcept>
#include "node.h"

/**
 * A singly linked list with references to both head and tail nodes.
 * This structure allows O(1) insertion at both ends.
 * Unlike a doubly linked list, traversal is unidirectional (head → tail).
 */
class DoubleEndedLinkedList {
public:
    Node* head = nullptr;  // Package-private in Java; public here for main access

    ~DoubleEndedLinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    /**
     * Inserts a new node at the start of the list.
     */
    void insert_first(char value) {
        Node* new_node = new Node(value);
        if (is_empty()) {
            head  = new_node;
            tail_ = new_node;
        } else {
            new_node->next = head;
            head           = new_node;
        }
    }

    /**
     * Inserts a new node after the node with the specified predecessor value.
     *
     * @throws std::runtime_error if the predecessor is not found.
     */
    void insert_after(char pred_value, char value) {
        Node* pred = head;
        while (pred != nullptr && pred->data != pred_value) {
            pred = pred->next;
        }
        if (pred != nullptr) {
            Node* new_node   = new Node(value);
            new_node->next   = pred->next;
            pred->next       = new_node;
            if (pred == tail_) tail_ = new_node;
        } else {
            throw std::runtime_error("Predecessor not found");
        }
    }

    /**
     * Inserts a new node at the end of the list (O(1) via tail pointer).
     */
    void insert_last(char value) {
        Node* new_node = new Node(value);
        if (is_empty()) {
            head  = new_node;
            tail_ = new_node;
        } else {
            tail_->next = new_node;
            tail_       = new_node;
        }
    }

    /**
     * Deletes the first node from this list.
     *
     * @throws std::runtime_error if the list is empty.
     */
    void delete_first() {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        if (head == tail_) {
            delete head;
            head  = nullptr;
            tail_ = nullptr;
        } else {
            Node* temp = head;
            head       = head->next;
            delete temp;
        }
    }

    /**
     * Deletes the first occurrence of a node with the specified value.
     *
     * @throws std::runtime_error if the list is empty or the value is not found.
     */
    void delete_by_value(char value) {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        if (head->data == value) {
            Node* temp = head;
            if (head == tail_) {
                head  = nullptr;
                tail_ = nullptr;
            } else {
                head = head->next;
            }
            delete temp;
            return;
        }
        Node* pred = head;
        Node* temp = head->next;
        while (temp != nullptr && temp->data != value) {
            pred = pred->next;
            temp = temp->next;
        }
        if (temp != nullptr) {
            pred->next = temp->next;
            if (pred->next == nullptr) tail_ = pred;
            delete temp;
        } else {
            throw std::runtime_error(
                std::string("Value '") + value + "' not found in the list.");
        }
    }

    /**
     * Deletes the last node from this list.
     *
     * @throws std::runtime_error if the list is empty.
     */
    void delete_last() {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        if (head == tail_) {
            delete head;
            head  = nullptr;
            tail_ = nullptr;
        } else {
            Node* pred = head;
            while (pred->next != tail_) {
                pred = pred->next;
            }
            delete tail_;
            pred->next = nullptr;
            tail_      = pred;
        }
    }

    bool is_empty() const { return head == nullptr; }

private:
    Node* tail_ = nullptr;
};
