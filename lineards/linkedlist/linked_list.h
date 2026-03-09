#pragma once

#include <stdexcept>
#include "node.h"

/**
 * A singly linked list implementation for storing characters.
 */
class LinkedList {
public:
    Node* head = nullptr;  // Package-private in Java; public here for main access

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    /**
     * Checks whether the linked list is empty.
     */
    bool is_empty() const { return head == nullptr; }

    /**
     * Inserts a new node with the specified value at the beginning of this list.
     */
    void insert_first(char value) {
        Node* node = new Node(value);
        node->next = head;
        head       = node;
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
            Node* node = new Node(value);
            node->next = pred->next;
            pred->next = node;
        } else {
            throw std::runtime_error("Predecessor not found");
        }
    }

    /**
     * Inserts a new node with the specified value at the end of this list.
     */
    void insert_last(char value) {
        if (is_empty()) {
            head = new Node(value);
            return;
        }
        Node* pred = head;
        while (pred->next != nullptr) {
            pred = pred->next;
        }
        pred->next = new Node(value);
    }

    /**
     * Deletes the first node from this list.
     *
     * @throws std::runtime_error if the list is empty.
     */
    void delete_first() {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        Node* temp = head;
        head       = head->next;
        delete temp;
    }

    /**
     * Deletes the first found node with the specified value.
     *
     * @throws std::runtime_error if the list is empty or the value is not found.
     */
    void delete_by_value(char value) {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        if (head->data == value) {
            Node* temp = head;
            head       = head->next;
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
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            return;
        }
        Node* pred = head;
        Node* temp = head->next;
        while (temp->next != nullptr) {
            pred = pred->next;
            temp = temp->next;
        }
        pred->next = nullptr;
        delete temp;
    }
};
