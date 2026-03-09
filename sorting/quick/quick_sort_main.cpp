/**
 * Demonstrates the Quick Sort algorithm using different partition schemes.
 *
 * Both Lomuto and Hoare produce the same sorted output. The key difference is
 * how they partition:
 * - Lomuto: picks the last element as pivot; pivot lands at its final position.
 * - Hoare: picks the first element as pivot; pivot may not land at its final
 *   position, so both recursive calls must include the partition index.
 *
 * Time complexity: O(n log n) average, O(n^2) worst case (already sorted input
 * with a bad pivot choice).
 * Space complexity: O(log n) average call stack depth.
 */

#include <iostream>
#include <stdexcept>
#include <vector>

#include "quick_sort_lomuto.h"
#include "quick_sort_hoare.h"

namespace {

enum class PartitionScheme {
    LOMUTO,
    HOARE
};

/**
 * Sorts an array using the specified Quick Sort partition scheme.
 */
void sort(std::vector<int>& array, PartitionScheme scheme) {
    if (array.size() <= 1) return;
    switch (scheme) {
        case PartitionScheme::LOMUTO:
            QuickSortLomuto::sort(array, 0, static_cast<int>(array.size()) - 1);
            break;
        case PartitionScheme::HOARE:
            QuickSortHoare::sort(array, 0, static_cast<int>(array.size()) - 1);
            break;
        default:
            throw std::invalid_argument("Unsupported partition scheme.");
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

    sort(array, PartitionScheme::LOMUTO);

    std::cout << "\n\nSorted Array:\n";
    print_array(array);
    std::cout << "\n";

    return 0;
}
