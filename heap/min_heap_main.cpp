/**
 * Demonstrates MinHeap operations: insert, peek_min, extract_min (which
 * yields the values in ascending order), and remove. Output is annotated
 * so the trace reads as a tour of the operations.
 */

#include <iostream>

#include "min_heap.h"

int main() {
    MinHeap heap(15);

    std::cout << "=== Insert ===\n";
    for (int value : {5, 3, 8, 1, 9, 2, 7}) {
        heap.insert(value);
        std::cout << "insert(" << value << ")   min=" << heap.peek_min()
                  << "   size=" << heap.size() << "\n";
    }

    std::cout << "\n=== Remove ===\n";
    // Remove the node at index 2 to show the replacement being sifted.
    std::cout << "remove(index 2)\n";
    heap.remove(2);
    std::cout << "min=" << heap.peek_min() << "   size=" << heap.size()
              << "\n";

    std::cout << "\n=== Extract-min (ascending) ===\n";
    bool first = true;
    while (!heap.is_empty()) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        std::cout << heap.extract_min();
    }
    std::cout << "\n";
    return 0;
}
