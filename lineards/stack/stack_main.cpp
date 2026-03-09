/**
 * Demonstrates the functionalities of different stack implementations.
 */

#include <iostream>
#include "static_array_stack.h"
#include "dynamic_array_stack.h"
#include "linked_list_stack.h"

void demo_static_array_stack() {
    std::cout << "Demo: StaticArrayStack\n";
    StaticArrayStack static_stack(3);
    std::cout << "Push A, B, C\n";
    static_stack.push('A');
    static_stack.push('B');
    static_stack.push('C');

    try {
        std::cout << "Pushing D...\n";
        static_stack.push('D');  // Should throw — stack is full
    } catch (const std::overflow_error& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "Pop from stack: " << static_stack.pop() << "\n";    // C
    std::cout << "Peek from stack: " << static_stack.peek() << "\n";  // B
    std::cout << "Is empty: " << std::boolalpha << static_stack.is_empty() << "\n";  // false
    static_stack.pop();
    static_stack.pop();
    std::cout << "Is empty after popping all elements: "
              << static_stack.is_empty() << "\n\n";  // true
}

void demo_dynamic_array_stack() {
    std::cout << "Demo: DynamicArrayStack\n";
    DynamicArrayStack dynamic_stack(2);
    std::cout << "Push W, X, Y, Z\n";
    dynamic_stack.push('W');
    dynamic_stack.push('X');
    dynamic_stack.push('Y');  // Triggers resize
    dynamic_stack.push('Z');

    std::cout << "Pop from stack: " << dynamic_stack.pop() << "\n";    // Z
    std::cout << "Peek from stack: " << dynamic_stack.peek() << "\n";  // Y
    std::cout << "Is empty: " << dynamic_stack.is_empty() << "\n";     // false
    dynamic_stack.pop();
    dynamic_stack.pop();
    dynamic_stack.pop();
    std::cout << "Is empty after popping all elements: "
              << dynamic_stack.is_empty() << "\n\n";  // true
}

void demo_linked_list_stack() {
    std::cout << "Demo: LinkedListStack\n";
    LinkedListStack linked_list_stack;
    std::cout << "Push 1, 2, 3\n";
    linked_list_stack.push('1');
    linked_list_stack.push('2');
    linked_list_stack.push('3');

    std::cout << "Pop from stack: " << linked_list_stack.pop() << "\n";    // 3
    std::cout << "Peek from stack: " << linked_list_stack.peek() << "\n";  // 2
    std::cout << "Is empty: " << linked_list_stack.is_empty() << "\n";     // false
    linked_list_stack.pop();
    linked_list_stack.pop();
    std::cout << "Is empty after popping all elements: "
              << linked_list_stack.is_empty() << "\n\n";  // true
}

int main() {
    demo_static_array_stack();
    demo_dynamic_array_stack();
    demo_linked_list_stack();
    return 0;
}
