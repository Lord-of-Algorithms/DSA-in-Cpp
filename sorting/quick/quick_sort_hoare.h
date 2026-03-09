#pragma once

#include <algorithm>
#include <vector>

/**
 * Implements Quick Sort using the Hoare partition scheme.
 *
 * The first element of each sub-array is chosen as the pivot. Two pointers
 * scan inward from both ends and swap elements that are on the wrong side.
 * Unlike Lomuto, the pivot is NOT guaranteed to land at its final position
 * after partitioning, so both recursive calls must include the partition index.
 */
class QuickSortHoare {
public:
    /**
     * Sorts the segment array[low..high] using the Hoare partition scheme.
     */
    static void sort(std::vector<int>& array, int low, int high) {
        if (low < high) {
            // Partition the array and obtain the index dividing the two partitions
            int partition_index = partition(array, low, high);

            // Unlike Lomuto, the partition index is not the pivot's final position,
            // so both calls must include it in their respective ranges.
            sort(array, low, partition_index);
            sort(array, partition_index + 1, high);
        }
    }

private:
    /**
     * Partitions array[low..high] using the first element as pivot.
     * Two pointers scan inward; elements are swapped when i finds one >= pivot
     * and j finds one <= pivot.
     *
     * @return The last index of the lower partition (not necessarily the pivot's position).
     */
    static int partition(std::vector<int>& array, int low, int high) {
        int pivot = array[low];
        int i     = low - 1;
        int j     = high + 1;

        while (true) {
            // Move right while the element is less than the pivot
            do { ++i; } while (array[i] < pivot);
            // Move left while the element is greater than the pivot
            do { --j; } while (array[j] > pivot);

            if (i >= j) return j;  // Partitions have met — return the boundary index

            std::swap(array[i], array[j]);
        }
    }
};
