#pragma once

#include <algorithm>
#include <vector>

/**
 * Implements the Heap Sort algorithm using a recursive heapify.
 *
 * Heapify calls itself on the affected subtree after each swap, making the
 * recursion structure explicit and easier to follow step by step.
 */
class HeapSortRecursive {
public:
    /**
     * Sorts the provided array using the Heap Sort algorithm.
     */
    static void sort(std::vector<int>& array) {
        int n = static_cast<int>(array.size());
        build_heap(array, n);
        for (int i = n - 1; i > 0; --i) {
            // Swap the root (max element) with the last element of the unsorted part
            std::swap(array[0], array[i]);
            // Restore the heap property on the reduced heap
            heapify(array, 0, i);
        }
    }

private:
    /**
     * Transforms the array into a max heap by heapifying all non-leaf nodes bottom-up.
     */
    static void build_heap(std::vector<int>& array, int size) {
        for (int i = size / 2 - 1; i >= 0; --i) {
            heapify(array, i, size);
        }
    }

    /**
     * Ensures the max-heap property for the subtree rooted at index i.
     * If the root is smaller than its largest child, swaps them and
     * recursively heapifies the affected subtree.
     */
    static void heapify(std::vector<int>& array, int i, int size) {
        int examined          = array[i];
        int largest_child     = get_largest_child_index(array, i, size);
        if (largest_child < size && examined < array[largest_child]) {
            // Perform the swap
            array[i]             = array[largest_child];
            array[largest_child] = examined;
            // Recursively heapify the affected subtree
            heapify(array, largest_child, size);
        }
    }

    /**
     * Returns the index of the larger child of the node at the given index.
     */
    static int get_largest_child_index(const std::vector<int>& array, int index, int size) {
        int left  = 2 * index + 1;
        int right = left + 1;
        if (right < size && array[left] < array[right]) {
            return right;  // Right child is larger
        }
        return left;  // Left child is larger or the only child
    }
};
