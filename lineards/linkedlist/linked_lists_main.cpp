/**
 * Demonstrates the functionality of LinkedList and DoubleEndedLinkedList.
 */

#include <iostream>
#include "linked_list.h"
#include "double_ended_linked_list.h"

// Helper: print the linked list from a given head node to null
void print_list(const Node* head) {
    const Node* current = head;
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

void demo_double_linked_list() {
    std::cout << "\nDemo DoubleLinkedList:\n";
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

int main() {
    demo_linked_list();
    demo_double_linked_list();
    return 0;
}
