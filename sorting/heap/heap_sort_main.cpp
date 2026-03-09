/**
 * Demonstrates the Heap Sort algorithm in both iterative and recursive forms.
 *
 * Both variants produce identical sorted output. The difference is in heapify:
 * - Iterative: walks down the tree in a loop — no call-stack overhead.
 * - Recursive: calls itself on the affected subtree — makes the structure more explicit.
 *
 * Time complexity: O(n log n) in all cases.
 * Space complexity: O(1) for iterative heapify, O(log n) call stack for recursive.
 */

#include <iostream>
#include <stdexcept>
#include <vector>

#include "heap_sort_iterative.h"
#include "heap_sort_recursive.h"

namespace {

enum class SortNature {
    ITERATIVE,
    RECURSIVE
};

/**
 * Sorts an array using either the iterative or recursive Heap Sort.
 */
void sort(std::vector<int>& array, SortNature nature) {
    if (array.size() <= 1) return;
    switch (nature) {
        case SortNature::ITERATIVE: HeapSortIterative::sort(array); break;
        case SortNature::RECURSIVE: HeapSortRecursive::sort(array); break;
        default: throw std::invalid_argument("Unsupported sort nature.");
    }
}

void print_array(const std::vector<int>& array) {
    for (int value : array) std::cout << value << " ";
}

} // namespace

int main() {
    std::vector<int> array = {64, 25, 12, 22, 11, 90, 18};

    std::cout << "Original Array:\n";
    print_array(array);

    sort(array, SortNature::RECURSIVE);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
