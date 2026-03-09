/**
 * Demonstrates the operations of a binary search tree.
 * Supports Standard (iterative/recursive), AVL, and Red-Black tree types.
 */

#include <iostream>
#include <memory>
#include "binary_tree_printer.h"
#include "bst/tree.h"
#include "bst/standard/binary_search_tree_iterative.h"
#include "bst/standard/binary_search_tree_recursive.h"
#include "bst/avl/avl_tree.h"
#include "bst/red_black/red_black_tree.h"
#include "traversal/pre_order_traversal.h"
#include "traversal/in_order_traversal.h"
#include "traversal/post_order_traversal.h"
#include "traversal/breadth_first_traversal.h"

namespace {

/**
 * Specifies the type of binary search tree to demonstrate.
 */
enum class BstType {
    StandardIterative,
    StandardRecursive,
    Avl,
    RedBlack
};

std::unique_ptr<Tree> create_tree(BstType type) {
    switch (type) {
        case BstType::StandardIterative: return std::make_unique<BinarySearchTreeIterative>();
        case BstType::StandardRecursive: return std::make_unique<BinarySearchTreeRecursive>();
        case BstType::Avl:               return std::make_unique<AvlTree>();
        case BstType::RedBlack:          return std::make_unique<RedBlackTree>();
    }
    throw std::invalid_argument("Unknown BstType");
}

void insert_keys_into_tree(Tree& tree, std::initializer_list<int> keys) {
    for (int key : keys) {
        std::cout << "Inserting key: " << key << "\n";
        tree.insert(key);
    }
}

void delete_keys_from_tree(Tree& tree, std::initializer_list<int> keys) {
    for (int key : keys) {
        std::cout << "Delete node with key: " << key << "\n";
        tree.delete_key(key);
        BinaryTreePrinter::print(tree.get_root());
    }
}

void search_keys_in_tree(Tree& tree, std::initializer_list<int> keys) {
    for (int key : keys) {
        std::cout << "Search for a node with key: " << key << "\n";
        std::cout << "Is found: " << std::boolalpha << (tree.search(key) != nullptr) << "\n";
    }
}

void demonstrate_traversals(Tree& tree) {
    // Iterative pre-order
    PreOrderTraversal pre_order(TraversalNature::Iterative);
    std::cout << "\nPre-order traversal: \n";
    pre_order.traverse(tree.get_root());

    // Recursive in-order
    InOrderTraversal in_order(TraversalNature::Recursive);
    std::cout << "\nIn-order traversal: \n";
    in_order.traverse(tree.get_root());

    // Recursive post-order
    PostOrderTraversal post_order(TraversalNature::Recursive);
    std::cout << "\nPost-order traversal: \n";
    post_order.traverse(tree.get_root());

    // Breadth-first
    BreadthFirstTraversal bfs;
    std::cout << "\nBreadth-first traversal: \n";
    bfs.traverse(tree.get_root());
}

}  // namespace

int main() {
    // Choose the tree type: StandardIterative, StandardRecursive, Avl, RedBlack
    auto tree = create_tree(BstType::RedBlack);

    // Insert keys
    insert_keys_into_tree(*tree, {20, 7, 9, 2, 40, 22, 70, 70, 25});

    // Display the tree after insertions
    std::cout << "Binary tree after insertions:";
    BinaryTreePrinter::print(tree->get_root());

    // Delete nodes and display tree after each deletion
    delete_keys_from_tree(*tree, {20, 40, 2, 7});

    // Search for keys
    search_keys_in_tree(*tree, {70, 44});

    // Demonstrate traversals
    demonstrate_traversals(*tree);

    std::cout << "\n";
    return 0;
}
