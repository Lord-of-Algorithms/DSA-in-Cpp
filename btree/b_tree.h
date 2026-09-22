#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * A B-tree of distinct integer keys.
 *
 * A B-tree is a balanced search tree whose nodes hold several keys at once.
 * The keys inside a node are kept in ascending order, and a node holding k
 * keys, unless it is a leaf, has k + 1 children: one for each gap between
 * two keys, and one at either end. Every key in a child lies in the range
 * its gap describes.
 *
 * The size of the nodes is set by the maximum degree m: the most children a
 * node may have. From it follow the two rules every node obeys:
 *
 *   * at most m - 1 keys — one more and the node splits;
 *   * at least ceil(m / 2) - 1 keys, for every node but the root — one
 *     fewer and the node borrows a key from a neighbour or merges with one.
 *
 * Every leaf sits at the same depth. A new key joins a leaf that already
 * exists, and the tree grows taller only when its root splits, which adds a
 * level above the whole tree at once. Deleting runs the same machinery
 * backwards, and the tree grows shorter only when a merge empties the root.
 *
 * contains, insert and remove each walk one path from the root to a leaf
 * and back, so all three are O(log n).
 *
 * Note: the delete operation is named remove() because `delete` is a C++
 * keyword.
 */
class BTree {
public:
    /**
     * Creates an empty B-tree whose nodes may have up to max_degree
     * children. Throws std::invalid_argument if max_degree is less than 3.
     */
    explicit BTree(int max_degree)
        : max_keys_(max_degree - 1),
          min_keys_((max_degree + 1) / 2 - 1), // ceil(max_degree / 2) - 1
          root_(std::make_unique<Node>()) {
        if (max_degree < 3) {
            throw std::invalid_argument("Max degree must be at least 3");
        }
    }

    // Returns true if the tree holds no keys.
    bool is_empty() const { return root_->keys.empty(); }

    // Returns true if the tree holds key.
    bool contains(int key) const {
        const Node* node = root_.get();
        while (true) {
            int i = lower_bound(node, key);
            if (i < key_count(node) && node->keys[i] == key) {
                return true;
            }
            if (node->is_leaf()) {
                return false;
            }
            // Descend through the gap the key falls into.
            node = node->children[i].get();
        }
    }

    /**
     * Inserts key. The key goes into the leaf it belongs in; if that leaf
     * now holds one key too many it splits, and the split can repeat all
     * the way up to the root. Returns false if the key was already in the
     * tree.
     */
    bool insert(int key) {
        // Step 1: walk down to the leaf the key belongs in, remembering the
        // path so the splits can climb back up it.
        std::vector<Node*> path;
        Node* node = root_.get();
        while (true) {
            int i = lower_bound(node, key);
            if (i < key_count(node) && node->keys[i] == key) {
                return false; // already in the tree
            }
            if (node->is_leaf()) {
                // Step 2: the leaf makes room and takes the key.
                node->keys.insert(node->keys.begin() + i, key);
                break;
            }
            path.push_back(node);
            node = node->children[i].get();
        }

        // Step 3: while a node holds one key too many, split it. Its middle
        // key goes up into the parent, which may overflow in its turn.
        while (key_count(node) > max_keys_) {
            Node* parent = nullptr;
            if (!path.empty()) {
                parent = path.back();
                path.pop_back();
            }
            split(node, parent);
            if (parent == nullptr) {
                break; // the root split: the tree is one level taller
            }
            node = parent;
        }
        return true;
    }

    /**
     * Deletes key. A key can only leave a leaf, so a key inside an internal
     * node first trades places with its successor. Removing it may leave a
     * node below the minimum, which is repaired from the bottom up by
     * borrowing a key from a neighbour or merging with one. Returns false if
     * the key was not in the tree.
     */
    bool remove(int key) {
        // Step 1: walk down to the key, remembering the path so the repair
        // can climb back up it.
        std::vector<Node*> path;
        Node* node = root_.get();
        int index;
        while (true) {
            index = lower_bound(node, key);
            if (index < key_count(node) && node->keys[index] == key) {
                break;
            }
            if (node->is_leaf()) {
                return false; // not in the tree
            }
            path.push_back(node);
            node = node->children[index].get();
        }

        // Step 2: a key inside an internal node trades places with its
        // successor — the leftmost key of the subtree to its right — so
        // that it can leave from a leaf.
        if (!node->is_leaf()) {
            Node* holder = node;
            path.push_back(node);
            node = node->children[index + 1].get();
            while (!node->is_leaf()) {
                path.push_back(node);
                node = node->children[0].get();
            }
            holder->keys[index] = node->keys[0];
            node->keys[0] = key;
            index = 0;
        }

        // Step 3: the leaf gives the key up. It may now be below the
        // minimum — even empty — and stays that way until Step 4 repairs it.
        node->keys.erase(node->keys.begin() + index);

        // Step 4: repair from the bottom up. A node below the minimum borrows
        // a spare key from a neighbour, through the parent; if neither
        // neighbour has one, it merges with a neighbour and the key that
        // separated them, which takes a key from the parent — so the repair
        // continues one level higher.
        while (node != root_.get() && key_count(node) < min_keys_) {
            Node* parent = path.back();
            path.pop_back();
            int at = index_of(parent, node);
            Node* left = at > 0 ? parent->children[at - 1].get() : nullptr;
            Node* right = at < child_count(parent) - 1
                              ? parent->children[at + 1].get()
                              : nullptr;

            if (left != nullptr && key_count(left) > min_keys_) {
                borrow_from_left(parent, at);
            } else if (right != nullptr && key_count(right) > min_keys_) {
                borrow_from_right(parent, at);
            } else {
                // Merge with the left neighbour if there is one.
                int first = at > 0 ? at - 1 : at;
                merge(parent, first);
                if (parent == root_.get() && parent->keys.empty()) {
                    // The tree is one level shorter.
                    std::unique_ptr<Node> new_root =
                        std::move(parent->children[0]);
                    root_ = std::move(new_root);
                    break;
                }
            }
            node = parent;
        }
        return true;
    }

private:
    // Draws the tree, so it reaches the nodes.
    friend class BTreePrinter;

    // A node: its keys in ascending order and, unless it is a leaf, one more
    // child than it has keys. Each node owns its children.
    struct Node {
        std::vector<int> keys;
        std::vector<std::unique_ptr<Node>> children;

        bool is_leaf() const { return children.empty(); }
    };

    int max_keys_;
    int min_keys_;
    std::unique_ptr<Node> root_;

    // Number of keys and of children in a node, as ints.
    static int key_count(const Node* node) {
        return static_cast<int>(node->keys.size());
    }

    static int child_count(const Node* node) {
        return static_cast<int>(node->children.size());
    }

    // Index of the first key in the node that is not smaller than key — the
    // key itself if the node holds it, otherwise the gap it falls into.
    static int lower_bound(const Node* node, int key) {
        int i = 0;
        while (i < key_count(node) && node->keys[i] < key) {
            i++;
        }
        return i;
    }

    // Position of child among the parent's children.
    static int index_of(const Node* parent, const Node* child) {
        int i = 0;
        while (parent->children[i].get() != child) {
            i++;
        }
        return i;
    }

    // Splits an overfull node in two around its middle key, which moves up
    // into the parent. With an even number of keys there are two middles;
    // the right-hand one goes up, leaving the extra key in the left half. A
    // split root grows a new root above it.
    void split(Node* node, Node* parent) {
        int mid = key_count(node) / 2;
        int middle_key = node->keys[mid];

        auto right = std::make_unique<Node>();
        for (int i = mid + 1; i < key_count(node); i++) {
            right->keys.push_back(node->keys[i]);
        }
        node->keys.resize(mid);
        if (!node->is_leaf()) {
            for (int i = mid + 1; i < child_count(node); i++) {
                right->children.push_back(std::move(node->children[i]));
            }
            node->children.resize(mid + 1);
        }

        if (parent == nullptr) {
            auto new_root = std::make_unique<Node>();
            new_root->children.push_back(std::move(root_));
            root_ = std::move(new_root);
            parent = root_.get();
        }
        int at = index_of(parent, node);
        parent->keys.insert(parent->keys.begin() + at, middle_key);
        parent->children.insert(parent->children.begin() + at + 1,
                                std::move(right));
    }

    // The separating key comes down from the parent to the front of the
    // child at index at; the left neighbour's last key goes up to replace
    // it.
    static void borrow_from_left(Node* parent, int at) {
        Node* node = parent->children[at].get();
        Node* left = parent->children[at - 1].get();
        node->keys.insert(node->keys.begin(), parent->keys[at - 1]);
        parent->keys[at - 1] = left->keys.back();
        left->keys.pop_back();
        if (!left->is_leaf()) {
            node->children.insert(node->children.begin(),
                                  std::move(left->children.back()));
            left->children.pop_back();
        }
    }

    // The separating key comes down from the parent to the end of the child
    // at index at; the right neighbour's first key goes up to replace it.
    static void borrow_from_right(Node* parent, int at) {
        Node* node = parent->children[at].get();
        Node* right = parent->children[at + 1].get();
        node->keys.push_back(parent->keys[at]);
        parent->keys[at] = right->keys.front();
        right->keys.erase(right->keys.begin());
        if (!right->is_leaf()) {
            node->children.push_back(std::move(right->children.front()));
            right->children.erase(right->children.begin());
        }
    }

    // Merges the child at index first + 1 into the child at index first,
    // with the key that separated them in between.
    static void merge(Node* parent, int first) {
        Node* left = parent->children[first].get();
        Node* right = parent->children[first + 1].get();
        left->keys.push_back(parent->keys[first]);
        parent->keys.erase(parent->keys.begin() + first);
        for (int key : right->keys) {
            left->keys.push_back(key);
        }
        for (auto& child : right->children) {
            left->children.push_back(std::move(child));
        }
        parent->children.erase(parent->children.begin() + first + 1);
    }
};
