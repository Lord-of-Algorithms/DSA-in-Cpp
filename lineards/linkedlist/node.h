#pragma once

/**
 * A node in a singly linked list, holding a char value and a pointer to the next node.
 */
struct Node {
    char  data;
    Node* next = nullptr;

    explicit Node(char d) : data(d) {}
};
