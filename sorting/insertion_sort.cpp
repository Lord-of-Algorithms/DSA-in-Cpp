/**
 * Sorts an array using the Insertion Sort algorithm.
 *
 * Builds the sorted portion one element at a time by taking each new element
 * and shifting larger elements one position right until finding the correct
 * insertion point for the new element.
 *
 * Time complexity: O(n^2) average and worst case, O(n) best case (already sorted).
 * Space complexity: O(1) — sorts in place.
 */

#include <iostream>
#include <vector>

/**
 * Sorts the provided array using the Insertion Sort algorithm.
 */
static void insertion_sort(std::vector<int>& array) {
    int n = static_cast<int>(array.size());
    for (int i = 1; i < n; ++i) {
        int temp = array[i];

        // Move elements of array[0..i-1] that are greater than temp
        // one position ahead of their current position
        int j = i;
        while (j > 0 && array[j - 1] > temp) {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = temp;
    }
}

static void print_array(const std::vector<int>& array) {
    for (int value : array) std::cout << value << " ";
}

int main() {
    std::vector<int> array = {64, 25, 12, 22, 11, 90, 18};

    std::cout << "Original Array:\n";
    print_array(array);

    insertion_sort(array);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
