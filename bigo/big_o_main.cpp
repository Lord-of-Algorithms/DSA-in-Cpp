/**
 * Runs the five functions of big_o_examples.h on the arrays of the Big O
 * topic: 8 values, then the same 8 with 8 more appended.
 */

#include <algorithm>
#include <iostream>
#include <vector>

#include "big_o_examples.h"

namespace {

void print(const char* label, const std::vector<int>& values) {
    std::cout << label;
    for (std::size_t i = 0; i < values.size(); i++) {
        std::cout << (i == 0 ? "[" : ", ") << values[i];
    }
    std::cout << "]\n";
}

void run(const std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    int target = values[n - 1];
    std::vector<int> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    std::cout << "=== n = " << n << " ===\n";
    print("values:         ", values);
    std::cout << "read_at(5)      = " << read_at(values, 5) << "\n";
    std::cout << "linear_search(" << target << ") = "
              << linear_search(values, target) << "\n";
    print("sorted:         ", sorted);
    std::cout << "binary_search(" << target << ") = "
              << binary_search(sorted, target) << "\n";
    std::cout << "has_duplicate   = " << std::boolalpha
              << has_duplicate(values) << "\n\n";
}

}  // namespace

int main() {
    std::vector<int> values8 = {31, 18, 44, 23, 52, 37, 26, 58};
    std::vector<int> values16 = values8;
    values16.insert(values16.end(), {74, 63, 85, 69, 91, 66, 80, 97});
    run(values8);
    run(values16);
    std::cout << "print_coins(3):\n";
    print_coins(3);
    return 0;
}
