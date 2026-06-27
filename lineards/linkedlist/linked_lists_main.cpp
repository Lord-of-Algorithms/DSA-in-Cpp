/**
 * Demonstrates the functionality of LinkedList and DoubleEndedLinkedList.
 */

#include <iostream>
#include "linked_list.h"
#include "double_ended_linked_list.h"
#include "doubly_linked_list.h"
#include "double_ended_doubly_linked_list.h"

// Helper: print the linked list from a given head node to null
void print_list(const Node* head) {
    const Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "null\n";
}

// Helper: print the doubly linked list from a given head node to null
void print_list(const DoublyNode* head) {
    const DoublyNode* current = head;
    while (current != nullptr) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << "null\n";
}

void demo_linked_list() {
    std::cout << "Demo LinkedList:\n";
    LinkedList linked_list;

    linked_list.insert_first('C');
    linked_list.insert_first('B');
    linked_list.insert_first('A');
    std::cout << "Inserting C, B, A at the start...\n";
    print_list(linked_list.head);

    linked_list.insert_last('D');
    std::cout << "Inserting D at the end...\n";
    print_list(linked_list.head);

    linked_list.insert_after('B', 'E');
    std::cout << "Inserting E after B...\n";
    print_list(linked_list.head);

    linked_list.delete_first();
    std::cout << "Deleting the first element...\n";
    print_list(linked_list.head);

    linked_list.delete_by_value('E');
    std::cout << "Deleting E...\n";
    print_list(linked_list.head);

    linked_list.delete_last();
    std::cout << "Deleting the last element...\n";
    print_list(linked_list.head);
}

void demo_double_ended_linked_list() {
    std::cout << "\nDemo DoubleEndedLinkedList:\n";
    DoubleEndedLinkedList del;

    del.insert_first('1');
    del.insert_first('2');
    del.insert_first('3');
    std::cout << "Inserting 1, 2, 3 at the start...\n";
    print_list(del.head);

    del.insert_last('4');
    std::cout << "Inserting 4 at the end...\n";
    print_list(del.head);

    del.insert_after('2', '5');
    std::cout << "Inserting 5 after 2...\n";
    print_list(del.head);

    del.delete_first();
    std::cout << "Deleting the first element...\n";
    print_list(del.head);

    del.delete_by_value('5');
    std::cout << "Deleting 5...\n";
    print_list(del.head);

    del.delete_last();
    std::cout << "Deleting the last element...\n";
    print_list(del.head);
}

// Both doubly variants share the same interface, so one routine exercises either.
template <typename List>
void demo_doubly(const char* name, List& list) {
    std::cout << "\nDemo " << name << ":\n";

    list.insert_first('C');
    list.insert_first('B');
    list.insert_first('A');
    std::cout << "Inserting C, B, A at the start...\n";
    print_list(list.head);

    list.insert_last('D');
    std::cout << "Inserting D at the end...\n";
    print_list(list.head);

    list.insert_after('B', 'E');
    std::cout << "Inserting E after B...\n";
    print_list(list.head);

    list.delete_first();
    std::cout << "Deleting the first element...\n";
    print_list(list.head);

    list.delete_by_value('E');
    std::cout << "Deleting E...\n";
    print_list(list.head);

    list.delete_last();
    std::cout << "Deleting the last element...\n";
    print_list(list.head);
}

void demo_doubly_linked_list() {
    DoublyLinkedList list;
    demo_doubly("DoublyLinkedList", list);
}

void demo_double_ended_doubly_linked_list() {
    DoubleEndedDoublyLinkedList list;
    demo_doubly("DoubleEndedDoublyLinkedList", list);
}

int main() {
    demo_linked_list();
    demo_double_ended_linked_list();
    demo_doubly_linked_list();
    demo_double_ended_doubly_linked_list();
    return 0;
}
