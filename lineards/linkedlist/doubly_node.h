#pragma once

/**
 * A node in a doubly linked list, holding a char value and pointers to both
 * the next and the previous node.
 */
struct DoublyNode {
    char        data;
    DoublyNode* next = nullptr;
    DoublyNode* prev = nullptr;

    explicit DoublyNode(char d) : data(d) {}
};
