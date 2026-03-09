#pragma once

#include <algorithm>
#include <vector>

/**
 * Implements the Heap Sort algorithm using an iterative heapify.
 *
 * Heapify walks down the tree in a loop rather than via recursion, which avoids
 * call-stack overhead and is generally preferred in production code.
 */
class HeapSortIterative {
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
     * Uses an iterative approach: saves the root value, shifts larger children
     * upward, then places the original root value at its final position.
     */
    static void heapify(std::vector<int>& array, int i, int size) {
        int examined = array[i];
        while (i < size / 2) {  // Only non-leaf nodes need to be heapified
            int largest_child = get_largest_child_index(array, i, size);
            if (examined >= array[largest_child]) {
                break;  // Heap property is satisfied
            }
            // Overwrite the current node with its largest child's value
            array[i] = array[largest_child];
            // Move down to the largest child and continue
            i = largest_child;
        }
        // Place the original root value at the position determined by the last iteration
        array[i] = examined;
    }

    /**
     * Returns the index of the larger child of node at the given index.
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
