/**
 * Sorts an array using the Selection Sort algorithm.
 *
 * Divides the array into a sorted and an unsorted region. On each pass it finds
 * the smallest element in the unsorted region and swaps it into the first
 * position of that region, growing the sorted region by one.
 *
 * Time complexity: O(n^2) in all cases.
 * Space complexity: O(1) — sorts in place.
 */

#include <algorithm>
#include <iostream>
#include <vector>

/**
 * Sorts the provided array using the Selection Sort algorithm.
 */
static void selection_sort(std::vector<int>& array) {
    int n = static_cast<int>(array.size());
    // One by one move the boundary of the unsorted part
    for (int i = 0; i < n - 1; ++i) {
        // Assume the first element of the unsorted part is the minimum
        int min_index = i;
        // Check the rest of the array to find the true minimum
        for (int j = i + 1; j < n; ++j) {
            if (array[j] < array[min_index]) {
                min_index = j;  // Found a new minimum, remember its index
            }
        }
        // Swap the found minimum element with the first element of the unsorted part
        std::swap(array[min_index], array[i]);
    }
}

static void print_array(const std::vector<int>& array) {
    for (int value : array) std::cout << value << " ";
}

int main() {
    std::vector<int> array = {64, 25, 12, 22, 11, 90, 18};

    std::cout << "Original Array:\n";
    print_array(array);

    selection_sort(array);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
