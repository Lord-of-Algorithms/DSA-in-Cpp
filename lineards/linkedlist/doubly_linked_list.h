#pragma once

#include <stdexcept>
#include <string>
#include "doubly_node.h"

/**
 * A doubly linked list with a reference to the head only.
 * Each node links to both its next and its previous node, so the list can be
 * traversed in either direction. Without a tail reference, reaching the last
 * node means walking from the head, so insertion and deletion at the end are O(n).
 */
class DoublyLinkedList {
public:
    DoublyNode* head = nullptr;  // Package-private in Java; public here for main access

    ~DoublyLinkedList() {
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
     * Inserts a new node with the specified value at the beginning of the list.
     */
    void insert_first(char value) {
        DoublyNode* new_node = new DoublyNode(value);
        if (!is_empty()) {
            // Link the new node and the old first node to each other
            new_node->next = head;
            head->prev     = new_node;
        }
        head = new_node;
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
        }
        pred->next = new_node;
    }

    /**
     * Inserts a new node with the specified value at the end of the list.
     * With only a head reference, this walks from the head to the last node.
     */
    void insert_last(char value) {
        DoublyNode* new_node = new DoublyNode(value);
        if (is_empty()) {
            head = new_node;
            return;
        }
        DoublyNode* pred = head;
        while (pred->next != nullptr) {
            pred = pred->next;
        }
        new_node->prev = pred;
        pred->next     = new_node;
    }

    /**
     * Deletes the first node from this list.
     *
     * @throws std::runtime_error if the list is empty.
     */
    void delete_first() {
        if (is_empty()) throw std::runtime_error("The list is empty.");
        DoublyNode* removed = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        delete removed;
    }

    /**
     * Deletes the first found node with the specified value.
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
        // Re-link the predecessor over the doomed node...
        cur->prev->next = cur->next;
        if (cur->next != nullptr) {
            // ...and the successor back, unless this was the last node
            cur->next->prev = cur->prev;
        }
        delete cur;
    }

    /**
     * Deletes the last node from this list.
     * With only a head reference, this walks from the head to the last node.
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
        DoublyNode* cur = head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        // The last node knows its own predecessor, so step straight back to it
        DoublyNode* removed = cur;
        cur = cur->prev;
        cur->next = nullptr;
        delete removed;
    }
};
