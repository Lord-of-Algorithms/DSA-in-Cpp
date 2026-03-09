/**
 * Demonstrates the union-find (disjoint-set union) data structure.
 *
 * Union-find efficiently tracks which elements belong to the same group
 * and supports two operations:
 *   find(x)          — returns the representative of x's group
 *   union_sets(x, y) — merges the groups of x and y
 *
 * Path compression (in find) and union by rank (in union_sets) together give
 * amortised near-constant time per operation.
 */

#include <iostream>
#include "union_find.h"

int main() {
    // Initialise UnionFind with 10 elements (0..9)
    UnionFind uf(10);

    // Perform union operations
    uf.union_sets(0, 1);
    uf.union_sets(2, 3);
    uf.union_sets(4, 5);
    uf.union_sets(6, 7);
    uf.union_sets(7, 8);
    uf.union_sets(0, 9);
    uf.union_sets(5, 9);

    // Output the representative of each element to show which set it belongs to
    for (int i = 0; i < 10; ++i) {
        std::cout << "Element " << i << " is in set: " << uf.find(i) << "\n";
    }

    // Check whether specific pairs of elements share the same set
    std::cout << "Elements 2 and 3 are in the same set: "
              << std::boolalpha << (uf.find(2) == uf.find(3)) << "\n";

    std::cout << "Elements 0 and 8 are in the same set: "
              << std::boolalpha << (uf.find(0) == uf.find(8)) << "\n";

    std::cout << "Elements 1 and 6 are in the same set: "
              << std::boolalpha << (uf.find(1) == uf.find(6)) << "\n";

    return 0;
}
