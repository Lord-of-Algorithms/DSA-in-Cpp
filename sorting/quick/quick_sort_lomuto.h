#pragma once

#include <algorithm>
#include <vector>

/**
 * Implements Quick Sort using the Lomuto partition scheme.
 *
 * The last element of each sub-array is chosen as the pivot. After partitioning,
 * the pivot is guaranteed to be in its final sorted position, so it is excluded
 * from both recursive calls.
 */
class QuickSortLomuto {
public:
    /**
     * Sorts the segment array[low..high] using the Lomuto partition scheme.
     */
    static void sort(std::vector<int>& array, int low, int high) {
        if (low < high) {
            // Partition the array around the pivot and get the pivot's final index
            int pivot_index = partition(array, low, high);
            sort(array, low, pivot_index - 1);   // Sort the sub-array before the pivot
            sort(array, pivot_index + 1, high);  // Sort the sub-array after the pivot
        }
    }

private:
    /**
     * Partitions array[low..high] using the last element as pivot.
     * Elements smaller than the pivot are moved to its left.
     *
     * @return The final index of the pivot element.
     */
    static int partition(std::vector<int>& array, int low, int high) {
        int pivot = array[high];
        int i     = low;  // Index of the smaller element boundary

        for (int j = low; j < high; ++j) {
            if (array[j] < pivot) {
                std::swap(array[i], array[j]);
                ++i;
            }
        }
        std::swap(array[i], array[high]);
        return i;  // Return the pivot's final position
    }
};
