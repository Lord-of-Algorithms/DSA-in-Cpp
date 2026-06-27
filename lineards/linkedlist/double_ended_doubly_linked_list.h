#pragma once

#include <stdexcept>
#include <string>
#include "doubly_node.h"

/**
 * A doubly linked list with references to both the head and tail nodes.
 * Each node links to both its next and its previous node, so the list can be
 * traversed in either direction. Keeping a tail reference makes insertion and
 * deletion at the end O(1), just like at the beginning - and unlike a singly
 * double-ended list, even deletion at the end is O(1), because the last node
 * knows its own predecessor through its prev reference.
 */
class DoubleEndedDoublyLinkedList {
public:
    DoublyNode* head = nullptr;  // Package-private in Java; public here for main access

    ~DoubleEndedDoublyLinkedList() {
        DoublyNode* current = head;
        while (current) {
            DoublyNode* next = current->next;
            delete current;
            current = next;
        }
    }

    /**
     * Checks whether the linked list is empty.
     */
    bool is_empty() const { return head == nullptr; }

    /**
     * Inserts a new node with the specified value at the start of the list.
     */
    void insert_first(char value) {
        DoublyNode* new_node = new DoublyNode(value);
        if (is_empty()) {
            head  = new_node;
            tail_ = new_node;
        } else {
            // Link the new node and the old first node to each other
            new_node->next = head;
            head->prev     = new_node;
            head           = new_node;
        }
    }

    /**
     * Inserts a new node after the node with the specified predecessor value.
     *
     * @throws std::runtime_error if the predecessor is not found.
     */
    void insert_after(char pred_value, char value) {
        DoublyNode* pred = head;
        while (pred != nullptr && pred->data != pred_value) {
            pred = pred->next;
        }
        if (pred == nullptr) {
            throw std::runtime_error("Predecessor not found");
        }
        DoublyNode* new_node = new DoublyNode(value);
        // Link the new node to both of its neighbours
        new_node->next = pred->next;
        new_node->prev = pred;
        if (pred->next != nullptr) {
            pred->next->prev = new_node;
        } else {
            // Inserting after the last node: the new node becomes the tail
            tail_ = new_node;
        }
        pred->next = new_node;
    }

    /**
     * Inserts a new node with the specified value at the end of the list
     * (O(1) via the tail pointer).
     */
    void insert_last(char value) {
        DoublyNode* new_node = new DoublyNode(value);
        if (is_empty()) {
            head  = new_node;
            tail_ = new_node;
        } else {
            // Link the new node back to the current last node,
            // then move the tail forward onto it
            new_node->prev = tail_;
            tail_->next    = new_node;
            tail_          = new_node;
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
            DoublyNode* removed = head;
            head       = head->next;
            head->prev = nullptr;
            delete removed;
        }
    }

    /**
     * Deletes the first occurrence of a node with the specified value.
     *
     * @throws std::runtime_error if the list is empty or the value is not found.
     */
    void delete_by_value(char value) {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        DoublyNode* cur = head;
        while (cur != nullptr && cur->data != value) {
            cur = cur->next;
        }
        if (cur == nullptr) {
            throw std::runtime_error(
                std::string("Value '") + value + "' not found in the list.");
        }
        if (cur == head) {
            delete_first();
            return;
        }
        if (cur == tail_) {
            delete_last();
            return;
        }
        // A node in the middle: re-link its two neighbours to each other
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
    }

    /**
     * Deletes the last node from this list.
     * The tail reference makes this O(1) - the last node's predecessor is just
     * tail->prev, so no walk is needed.
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
            // The last node knows its own predecessor, so step the tail
            // straight back to it and clear its forward link
            DoublyNode* removed = tail_;
            tail_       = tail_->prev;
            tail_->next = nullptr;
            delete removed;
        }
    }

private:
    DoublyNode* tail_ = nullptr;
};
