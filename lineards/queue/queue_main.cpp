/**
 * Demonstrates the functionalities of different queue implementations.
 */

#include <iostream>
#include "static_array_queue.h"
#include "dynamic_array_queue.h"
#include "linked_list_queue.h"

/**
 * Demonstrates basic queue operations (enqueue, dequeue, peek) on any Queue implementation.
 */
void demo_queue(Queue& queue) {
    try {
        queue.enqueue('A');
        queue.enqueue('B');
        queue.enqueue('C');
        std::cout << "Enqueuing A, B, and C...\n";
        std::cout << "Peek: "    << queue.peek()    << "\n";
        std::cout << "Dequeue: " << queue.dequeue() << "\n";
        std::cout << "Peek: "    << queue.peek()    << "\n";
        std::cout << "Dequeue: " << queue.dequeue() << "\n";
        std::cout << "Peek: "    << queue.peek()    << "\n";
        std::cout << "Empty: "   << std::boolalpha << queue.is_empty() << "\n";
        std::cout << "Dequeue: " << queue.dequeue() << "\n";
        std::cout << "Empty: "   << queue.is_empty() << "\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

int main() {
    std::cout << "Static Array Queue Demo:\n";
    StaticArrayQueue static_queue(5);
    demo_queue(static_queue);

    std::cout << "\nDynamic Array Queue Demo:\n";
    DynamicArrayQueue dynamic_queue(5);
    demo_queue(dynamic_queue);

    std::cout << "\nLinked List Queue Demo:\n";
    LinkedListQueue linked_list_queue;
    demo_queue(linked_list_queue);

    return 0;
}
