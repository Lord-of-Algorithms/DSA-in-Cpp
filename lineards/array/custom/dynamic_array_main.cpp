/**
 * Demonstrates the use of the DynamicArray class.
 */

#include <iostream>
#include "dynamic_array.h"

void print_array(const DynamicArray& da) {
    std::cout << "Dynamic Array (Size=" << da.size()
              << ", Capacity=" << da.capacity() << "): ";
    for (int i = 0; i < da.size(); ++i) {
        std::cout << da.get(i) << " ";
    }
    std::cout << "\n\n";
}

int main() {
    DynamicArray dynamicArray(5);

    // Adding elements to the dynamic array
    dynamicArray.add(10);
    dynamicArray.add(20);
    dynamicArray.add(30);
    std::cout << "Adding 10, 20, 30...\n";
    print_array(dynamicArray);

    // Inserting element at a specific index
    dynamicArray.add(1, 15);
    std::cout << "Inserting 15 at index 1...\n";
    print_array(dynamicArray);

    // Removing element from the array
    dynamicArray.remove(1);
    std::cout << "Removing element at index 1...\n";
    print_array(dynamicArray);

    // Accessing elements
    std::cout << "Element at index 0: " << dynamicArray.get(0) << "\n\n";

    // Adding elements to expand capacity
    dynamicArray.add(40);
    dynamicArray.add(50);
    dynamicArray.add(60);  // This should trigger a resize
    std::cout << "Adding more elements to trigger resize...\n";
    print_array(dynamicArray);

    // Removing elements to trigger shrink
    dynamicArray.remove(0);
    dynamicArray.remove(0);
    dynamicArray.remove(0);
    dynamicArray.remove(0);
    std::cout << "Removing elements to trigger shrink...\n";
    print_array(dynamicArray);

    return 0;
}
