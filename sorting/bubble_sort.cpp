/**
 * Sorts an array using the Bubble Sort algorithm.
 *
 * Repeatedly steps through the array, compares adjacent elements, and swaps
 * them if they are in the wrong order. The early-exit flag stops the loop as
 * soon as a pass produces no swaps, meaning the array is already sorted.
 *
 * Time complexity: O(n^2) average and worst case, O(n) best case (already sorted).
 * Space complexity: O(1) — sorts in place.
 */

#include <algorithm>
#include <iostream>
#include <vector>

/**
 * Sorts the provided array using the Bubble Sort algorithm.
 */
static void bubble_sort(std::vector<int>& array) {
    int n = static_cast<int>(array.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            // Compare adjacent elements
            if (array[j] > array[j + 1]) {
                // Swap if they are in the wrong order
                std::swap(array[j], array[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped, the array is sorted
        if (!swapped) break;
    }
}

static void print_array(const std::vector<int>& array) {
    for (int value : array) std::cout << value << " ";
}

int main() {
    std::vector<int> array = {64, 34, 25, 12, 22, 11, 90};

    std::cout << "Original Array:\n";
    print_array(array);

    bubble_sort(array);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
