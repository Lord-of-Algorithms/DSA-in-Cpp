#pragma once

#include <memory>
#include <string>
#include <vector>

#include "trie_node.h"

/**
 * A trie (pronounced "try"): a tree-shaped data structure where each node
 * holds a single character, and a stored word corresponds to a path from
 * the root to a node flagged as end-of-word.
 *
 * Why a trie? Two operations a trie does extremely well that std::set or
 * std::unordered_set cannot:
 *
 *   * Prefix lookup — "give me every word that starts with 'ca'" runs in
 *     O(L + K) where L is the prefix length and K is the number of
 *     matches. A hash set would have to scan every entry.
 *
 *   * Ordered enumeration — depth-first traversal returns words in a
 *     deterministic order. This implementation uses std::map for the
 *     children, so output is alphabetical for free (see TrieNode).
 *
 * Complexity:
 *
 *     L = length of the word / prefix
 *     N = total words stored
 *     K = matches under a given prefix
 *     A = alphabet branching factor at each node (≤ 26 for English)
 *
 *     insert       O(L log A)
 *     remove       O(L log A)
 *     search       O(L log A)
 *     starts_with  O((L + K) log A)
 *     space        O(N * L) worst case; much less when prefixes overlap
 *
 * The log A factor comes from std::map lookups at each node and is
 * effectively constant for bounded alphabets. Swap children's container
 * type to std::unordered_map in TrieNode to get O(1) average lookups at
 * the cost of non-deterministic iteration order.
 *
 * Note: the delete operation is named remove() because `delete` is a
 * reserved keyword in C++.
 *
 * Used in: autocomplete keyboards, spell-check and "did-you-mean?",
 * T9 / predictive text, IP-routing longest-prefix match, DNS lookups.
 */
class Trie {
public:
    Trie() : root_(std::make_unique<TrieNode>('\0', nullptr)), word_count_(0) {}

    /** True iff no words are stored. */
    bool is_empty() const { return word_count_ == 0; }

    /** Total number of stored words. */
    int size() const { return word_count_; }

    /** Remove every word. */
    void clear() {
        root_->children.clear();
        word_count_ = 0;
    }

    /**
     * Insert `word` into the trie. No-op if the word is already present.
     * Empty words are ignored.
     */
    void insert(const std::string& word) {
        if (word.empty()) return;
        TrieNode* node = root_.get();
        for (char c : word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                // If the child for this character is missing, create it.
                // Shared prefixes share nodes — this is the heart of the
                // trie's memory advantage.
                auto result = node->children.emplace(
                    c, std::make_unique<TrieNode>(c, node));
                node = result.first->second.get();
            } else {
                node = it->second.get();
            }
        }
        // Mark the final node as end-of-word. Only bump word_count_ if we
        // transitioned the flag from false to true — re-inserting an
        // existing word is a no-op.
        if (!node->is_end_of_word) {
            node->is_end_of_word = true;
            ++word_count_;
        }
    }

    /**
     * Check whether `word` is stored in the trie. "Stored" means the path
     * exists AND the final node is flagged end-of-word. search("cat")
     * returns false if only "catch" is stored, even though the "cat" path
     * is reachable as a prefix.
     */
    bool search(const std::string& word) const {
        if (word.empty()) return false;
        const TrieNode* node = find_terminal_node(word);
        return node != nullptr && node->is_end_of_word;
    }

    /**
     * Remove `word` from the trie and remove any nodes that became orphaned
     * (no longer end-of-word and no remaining children). Returns true if a
     * word was actually removed, false if `word` was never in the trie.
     *
     * The cleanup phase runs after the end-of-word flag is cleared: walk
     * back up from the deleted leaf and remove every node that has no
     * end-of-word flag AND no children. Stop at the first node that's
     * still part of another word's path — either it carries its own
     * end-of-word flag, or it has another child branching off. Removing
     * "cab" from a trie that also stores "cat" removes only the 'b' node;
     * the shared "c-a" path stays because 'a' still has its 't' child.
     */
    bool remove(const std::string& word) {
        if (word.empty()) return false;
        // find_terminal_node returns const TrieNode*; we const_cast because
        // the trie owns its nodes through root_ and we're about to mutate
        // them. Standard C++ idiom for sharing a const helper between
        // const-correct read APIs and mutating internals.
        TrieNode* node = const_cast<TrieNode*>(find_terminal_node(word));
        if (node == nullptr || !node->is_end_of_word) return false;

        node->is_end_of_word = false;
        --word_count_;

        // Cleanup walk: remove the deleted leaf and its empty ancestors
        // until we hit a node that's still useful.
        while (!node->is_root() && !node->is_end_of_word && node->children.empty()) {
            TrieNode* parent = node->parent;
            parent->children.erase(node->character);  // destroys `node`
            node = parent;
        }
        return true;
    }

    /**
     * Return every word stored in the trie that begins with `prefix`.
     * Returns an empty vector if the prefix can't be walked (a
     * non-existent prefix means no matches by definition).
     *
     * An empty prefix matches every word.
     *
     * Algorithm: descend along the prefix to find its terminal node, then
     * DFS the subtree rooted there to collect every end-of-word
     * descendant. Each descendant uniquely identifies one stored word —
     * the word string is the path from root to that descendant.
     */
    std::vector<std::string> starts_with(const std::string& prefix) const {
        std::vector<std::string> matches;
        const TrieNode* prefix_node = prefix.empty()
            ? root_.get()
            : find_terminal_node(prefix);
        if (prefix_node == nullptr) return matches;

        std::string path = prefix;
        collect_words(prefix_node, path, matches);
        return matches;
    }

    /**
     * Return every word stored in the trie, in alphabetical order (a
     * consequence of std::map's sorted iteration). Useful for tests and
     * debugging.
     */
    std::vector<std::string> words() const { return starts_with(""); }

private:
    std::unique_ptr<TrieNode> root_;
    int word_count_;

    /**
     * Walk the trie following `path` character-by-character and return the
     * terminal node, or nullptr if any character has no corresponding
     * child along the way.
     */
    const TrieNode* find_terminal_node(const std::string& path) const {
        const TrieNode* node = root_.get();
        for (char c : path) {
            auto it = node->children.find(c);
            if (it == node->children.end()) return nullptr;
            node = it->second.get();
        }
        return node;
    }

    /**
     * DFS helper for starts_with() and words(). Appends node->character to
     * `path` on the way down, snapshots `path` into `out` whenever the
     * current node is end-of-word, recurses into every child, and pops
     * the appended character on the way back up to leave `path` untouched
     * for the caller's next branch.
     */
    void collect_words(const TrieNode* node, std::string& path,
                       std::vector<std::string>& out) const {
        if (node->is_end_of_word) {
            out.push_back(path);
        }
        for (const auto& [c, child] : node->children) {
            path.push_back(c);
            collect_words(child.get(), path, out);
            path.pop_back();
        }
    }
};
