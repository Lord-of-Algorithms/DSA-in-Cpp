/**
 * Sorts an array using the Merge Sort algorithm.
 *
 * Recursively divides the array into halves until each sub-array has one element,
 * then merges the sorted halves back together. An auxiliary array is allocated once
 * and reused across all merge steps to avoid repeated allocation.
 *
 * Time complexity: O(n log n) in all cases.
 * Space complexity: O(n) for the auxiliary array.
 */

#include <iostream>
#include <vector>

/**
 * Merges two sorted halves of the array back into a single sorted segment.
 *
 * @param array    The original array containing all elements.
 * @param aux      Auxiliary array used as temporary storage during merging.
 * @param left     Starting index of the first sorted half.
 * @param right    Starting index of the second sorted half.
 * @param end      Ending index of the second sorted half (inclusive).
 */
static void merge_both_parts(std::vector<int>& array, std::vector<int>& aux,
                              int left, int right, int end) {
    int start       = left;
    int mid         = right - 1;
    int items_count = end - left + 1;
    int i = 0;

    // Merge elements and store in aux
    while (left <= mid && right <= end) {
        if (array[left] < array[right]) {
            aux[i++] = array[left++];
        } else {
            aux[i++] = array[right++];
        }
    }

    // Copy remaining elements from the first half
    while (left <= mid)  aux[i++] = array[left++];

    // Copy remaining elements from the second half
    while (right <= end) aux[i++] = array[right++];

    // Copy back the merged elements to the original array
    for (i = 0; i < items_count; ++i) {
        array[start + i] = aux[i];
    }
}

/**
 * Recursive helper that divides the array into halves, sorts them, and merges.
 */
static void merge_sort_rec(std::vector<int>& array, std::vector<int>& aux,
                            int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        merge_sort_rec(array, aux, start, mid);       // Sort the first half
        merge_sort_rec(array, aux, mid + 1, end);     // Sort the second half
        merge_both_parts(array, aux, start, mid + 1, end); // Merge sorted halves
    }
}

/**
 * Sorts the provided array using the Merge Sort algorithm.
 */
static void merge_sort(std::vector<int>& array) {
    if (array.size() <= 1) return;
    std::vector<int> aux(array.size());
    merge_sort_rec(array, aux, 0, static_cast<int>(array.size()) - 1);
}

static void print_array(const std::vector<int>& array) {
    for (int value : array) std::cout << value << " ";
}

int main() {
    std::vector<int> array = {64, 25, 12, 22, 11, 90, 18};

    std::cout << "Original Array:\n";
    print_array(array);

    merge_sort(array);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
