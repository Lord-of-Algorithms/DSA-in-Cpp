/**
 * Demonstrates Trie operations: insert, search, remove (with cleanup of
 * empty branches), and autocomplete (starts_with) on a small pool of
 * words. Output is annotated so the trace reads as a tour of the four
 * operations.
 */

#include <iostream>
#include <string>
#include <vector>

#include "trie.h"

namespace {

void print_words(const std::vector<std::string>& words) {
    std::cout << "[";
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << words[i];
    }
    std::cout << "]";
}

}  // namespace

int main() {
    std::cout << std::boolalpha;
    Trie trie;

    std::cout << "=== Insert ===\n";
    // Insertion order is intentionally jumbled — children are stored in a
    // std::map (sorted by key), so the OUTPUT order is independent of the
    // INPUT order. See TrieNode for the std::unordered_map alternative
    // (O(1) lookup, no order guarantee).
    for (const std::string& word : {"topology", "cat", "cap", "cabin",
                                    "catfish", "cab", "top", "catch"}) {
        trie.insert(word);
        std::cout << "insert(\"" << word << "\")   size=" << trie.size() << "\n";
    }

    std::cout << "\n=== Words ===\n";
    // Note: alphabetical order, not insertion order. std::map keeps children
    // sorted by key, and DFS visits them in sorted order, so the full list
    // comes out alphabetised regardless of insertion sequence.
    std::cout << "words() = ";
    print_words(trie.words());
    std::cout << "\n";

    std::cout << "\n=== Search ===\n";
    for (const std::string& word : {"cab", "cat", "catch", "ca", "catfishy", "topology"}) {
        std::cout << "search(\"" << word << "\") -> " << trie.search(word) << "\n";
    }
    // Note: search("ca") is false because "ca" is only a prefix here, not
    // a stored word. The end-of-word flag is the distinction.

    std::cout << "\n=== StartsWith (autocomplete) ===\n";
    for (const std::string& prefix : {"c", "ca", "cat", "to", "xy"}) {
        std::cout << "starts_with(\"" << prefix << "\") -> ";
        print_words(trie.starts_with(prefix));
        std::cout << "\n";
    }
    // "xy" diverges immediately -> empty vector.

    std::cout << "\n=== Remove ===\n";
    // Remove a word whose path is shared by longer words.
    // -> Unmark the end-of-word flag on its terminal node, but DON'T
    //    remove any nodes -- the path stays because there are children
    //    below it.
    // "cat" sits at the 't' node under c-a; 't' has children 'c' (catch)
    // and 'f' (catfish), so cleanup is a no-op.
    std::cout << "remove(\"cat\")   -> " << trie.remove("cat") << "\n";
    std::cout << "words() = ";
    print_words(trie.words());
    std::cout << "\n";
    std::cout << "search(\"cat\")     -> " << trie.search("cat") << "\n";
    std::cout << "search(\"catch\")   -> " << trie.search("catch") << "\n";
    std::cout << "search(\"catfish\") -> " << trie.search("catfish") << "\n";

    // Remove a word whose tail is unique -- cleanup DOES remove nodes.
    // "catch" is c-a-t-c-h. After unmarking 'h', the cleanup walk runs:
    //   * h: not end-of-word, no children -> remove.
    //   * c (under c-a-t): not end-of-word, no children (h was just
    //     removed) -> remove.
    //   * t (under c-a): not end-of-word, but has child 'f' (for catfish)
    //     -> STOP. The catfish path stays intact.
    std::cout << "remove(\"catch\") -> " << trie.remove("catch") << "\n";
    std::cout << "words() = ";
    print_words(trie.words());
    std::cout << "\n";

    // Try to remove something that isn't there.
    std::cout << "remove(\"xyz\")   -> " << trie.remove("xyz") << "\n";

    std::cout << "\n=== Final state ===\n";
    std::cout << "size=" << trie.size() << ", words=";
    print_words(trie.words());
    std::cout << "\n";

    return 0;
}
