/**
 * Demonstrates BTree: search, insertion — including a split that adds a
 * level — and deletion with each of the repairs it can need. The trees are
 * the worked examples from the app's B-tree topic, all of maximum degree 4.
 */

#include <iostream>

#include "b_tree.h"
#include "b_tree_printer.h"

// A tree of maximum degree 4 holding first, first + step, ..., last,
// inserted in ascending order.
static BTree build_tree(int first, int last, int step) {
    BTree tree(4);
    for (int key = first; key <= last; key += step) {
        tree.insert(key);
    }
    return tree;
}

/**
 * Keys 10, 15, ..., 65 fill both the root and the leaf where 70 belongs.
 * Inserting 70 overflows that leaf, which splits and sends 65 up; the root
 * overflows in its turn and splits too, and the tree grows a level.
 */
static void demonstrate_insert() {
    std::cout << "=== Insert ===\n";
    BTree tree = build_tree(10, 65, 5);
    std::cout << "After inserting 10, 15, ..., 65:\n";
    BTreePrinter::print(tree);
    std::cout << std::boolalpha << "contains(45) = " << tree.contains(45)
              << ", contains(47) = " << tree.contains(47) << "\n";

    tree.insert(70);
    std::cout << "\nInsert 70 — the leaf splits, then the root:"
                 " one level more.\n";
    BTreePrinter::print(tree);
}

/**
 * Three deletions, one for each way a delete can end:
 *
 *   * a leaf left below the minimum borrows from its neighbour;
 *   * two merges, one above the other, empty the root and the tree loses a
 *     level;
 *   * a key in an internal node trades places with its successor and leaves
 *     from a leaf.
 */
static void demonstrate_remove() {
    std::cout << "\n=== Delete: borrowing from a neighbour ===\n";
    BTree borrow = build_tree(10, 70, 10);
    BTreePrinter::print(borrow);
    borrow.remove(70);
    std::cout << "Delete 70 — its leaf empties and borrows through"
                 " the parent:\n";
    BTreePrinter::print(borrow);

    std::cout << "\n=== Delete: a merge that removes a level ===\n";
    BTree merge = build_tree(10, 70, 5);
    for (int key = 70; key >= 55; key -= 5) {
        merge.remove(key);
    }
    BTreePrinter::print(merge);
    merge.remove(40);
    std::cout << "Delete 40 — two merges empty the root: one level less.\n";
    BTreePrinter::print(merge);

    std::cout << "\n=== Delete: a key in an internal node ===\n";
    BTree internal = build_tree(10, 80, 10);
    BTreePrinter::print(internal);
    internal.remove(60);
    std::cout << "Delete 60 — it trades places with its successor 70 and leaves"
                 " from a leaf:\n";
    BTreePrinter::print(internal);
}

int main() {
    demonstrate_insert();
    demonstrate_remove();
    return 0;
}
