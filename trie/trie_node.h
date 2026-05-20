#pragma once

#include <map>
#include <memory>

/**
 * One node of a Trie. Each node holds a single character and a map of its
 * children (one child per outgoing character). A node also carries a flag
 * indicating whether the path from the root to this node spells a stored
 * word — without that flag the trie could not tell "cat" (a stored word)
 * from "cat" appearing only as a prefix of "catch".
 *
 * The root node is created with the sentinel character '\0' and is never
 * a stored word itself. Every other node holds the character that its
 * incoming edge corresponds to.
 *
 * Memory ownership:
 *
 * children is a std::map<char, std::unique_ptr<TrieNode>>, so each node
 * owns its children. Destroying a node automatically destroys its entire
 * subtree — no manual cleanup. parent is a raw, non-owning pointer (the
 * parent always outlives its children), null for the root.
 *
 * Children iteration order:
 *
 * std::map keeps keys in sorted order, so iterating children yields
 * children alphabetically. Iteration over a std::map is an in-order walk
 * of the underlying red-black tree, O(k) for k children.
 *
 * Trade-off vs std::unordered_map:
 *   * std::map           — O(log k) insert/find, O(k) sorted iteration.
 *                          Used here for deterministic, readable output.
 *   * std::unordered_map — O(1) average insert/find, but no order guarantee
 *                          on iteration; words()/starts_with() output would
 *                          vary between runs.
 *
 * For an alphabet bounded at ~26 letters per node, the log factor is
 * effectively constant, so the readability of sorted output trumps the
 * micro-optimisation.
 */
class TrieNode {
public:
    const char character;
    TrieNode* const parent;          // non-owning, nullptr for root
    bool is_end_of_word = false;
    std::map<char, std::unique_ptr<TrieNode>> children;

    TrieNode(char c, TrieNode* p) : character(c), parent(p) {}

    /** True iff this is the root sentinel (no parent, no character). */
    bool is_root() const { return parent == nullptr; }
};
