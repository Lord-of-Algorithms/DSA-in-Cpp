#pragma once

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "probe_type.h"

/**
 * A hash table that resolves collisions with open addressing: every entry lives
 * directly in the backing array, and collisions are handled by probing for the
 * next candidate slot. Two probe sequences are supported (linear and quadratic).
 *
 * Deletion uses a tombstone marker so that probe chains passing through a
 * removed slot are not broken. Keys are names (strings) and values are phone
 * numbers (strings).
 */
class OpenAddressingHashTable {
public:
    OpenAddressingHashTable(int capacity, ProbeType probe_type)
        : size_(0), probe_type_(probe_type) {
        if (capacity < 1) {
            throw std::invalid_argument("Initial capacity must be >= 1.");
        }
        // A prime capacity spreads the probe sequence across the whole table.
        capacity = is_prime(capacity) ? capacity : next_prime(capacity);
        table_.assign(capacity, Slot{});
    }

    /**
     * Inserts a new key-value pair, or updates the value if the key already exists.
     */
    void put(const std::string& key, const std::string& value) {
        // Resize before the table gets too full.
        if (static_cast<double>(size_ + 1) / static_cast<double>(table_.size()) > MAX_LOAD_FACTOR) {
            rehash();
        }

        int m = static_cast<int>(table_.size());
        int home = hash(key);
        int first_tombstone = -1;

        for (int i = 0; i < m; ++i) {
            int index = (home + probe(i)) % m;
            Slot& slot = table_[index];

            if (slot.state == State::Empty) {
                // An empty slot ends the probe chain: the key is not present.
                // Reuse the first tombstone seen along the way, if any.
                int target = (first_tombstone != -1) ? first_tombstone : index;
                table_[target] = Slot{key, value, State::Occupied};
                ++size_;
                return;
            } else if (slot.state == State::Deleted) {
                // Remember the first tombstone so the key can be placed there.
                if (first_tombstone == -1) {
                    first_tombstone = index;
                }
            } else if (slot.key == key) {
                // Key already present — update its value in place.
                slot.value = value;
                return;
            }
            // Otherwise the slot holds a different key: keep probing.
        }
        // Defensive guard — never reached while the invariants hold (prime size,
        // load factor <= 0.5); fails loudly instead of silently dropping the key.
        throw std::runtime_error("Hash table is full");
    }

    /**
     * Returns the value associated with the key, or nullopt if not found.
     */
    std::optional<std::string> get(const std::string& key) const {
        int m = static_cast<int>(table_.size());
        int home = hash(key);

        for (int i = 0; i < m; ++i) {
            int index = (home + probe(i)) % m;
            const Slot& slot = table_[index];

            if (slot.state == State::Empty) {
                return std::nullopt; // An empty slot ends the probe chain.
            }
            if (slot.state == State::Occupied && slot.key == key) {
                return slot.value;
            }
            // A tombstone or a different key — keep probing.
        }
        return std::nullopt;
    }

    /**
     * Removes a key-value pair, leaving a tombstone in its place so that probe
     * chains running through the slot are not broken.
     * @return true if the key was found and removed, false otherwise.
     */
    bool remove(const std::string& key) {
        int m = static_cast<int>(table_.size());
        int home = hash(key);

        for (int i = 0; i < m; ++i) {
            int index = (home + probe(i)) % m;
            Slot& slot = table_[index];

            if (slot.state == State::Empty) {
                return false; // An empty slot ends the probe chain.
            }
            if (slot.state == State::Occupied && slot.key == key) {
                slot.state = State::Deleted; // Tombstone, not an empty slot.
                slot.key.clear();
                slot.value.clear();
                --size_;
                return true;
            }
        }
        return false;
    }

    int size() const { return size_; }

    /**
     * Prints the backing array slot by slot.
     */
    void print() const {
        for (int i = 0; i < static_cast<int>(table_.size()); ++i) {
            std::cout << "[" << i << "] ";
            const Slot& slot = table_[i];
            switch (slot.state) {
                case State::Empty:    std::cout << "-\n"; break;
                case State::Deleted:  std::cout << "(deleted)\n"; break;
                case State::Occupied: std::cout << slot.key << " = " << slot.value << "\n"; break;
            }
        }
    }

private:
    // Open addressing degrades sharply as the table fills. Keeping the load
    // factor at most 0.5 also guarantees that quadratic probing on a prime-sized
    // table always finds a free slot.
    static constexpr double MAX_LOAD_FACTOR = 0.5;

    // The lifecycle state of a backing-array slot.
    enum class State { Empty, Occupied, Deleted };

    // -------------------------------------------------------------------------
    // Slot — a key-value pair stored directly in the backing array
    // -------------------------------------------------------------------------
    struct Slot {
        std::string key;
        std::string value;
        State       state = State::Empty;
    };

    std::vector<Slot> table_;
    int               size_;
    ProbeType         probe_type_;

    // Computes the home slot for a key using the division method. Any hash
    // function works here; open addressing fixes one so the probe sequence
    // stays the focus (see HashFunctionType in the chaining table).
    int hash(const std::string& key) const {
        std::size_t hash_code = std::hash<std::string>{}(key);
        return static_cast<int>(hash_code % table_.size());
    }

    // Computes the offset added to the home slot on the i-th probe.
    int probe(int i) const {
        switch (probe_type_) {
            case ProbeType::Linear:    return i;      // home, home+1, home+2, ...
            case ProbeType::Quadratic: return i * i;  // home, home+1, home+4, home+9, ...
            default:
                throw std::invalid_argument("Unknown Probe Type.");
        }
    }

    // Doubles the capacity (rounded up to a prime) and reinserts every active
    // entry. Tombstones are discarded in the process.
    void rehash() {
        std::vector<Slot> old_table = std::move(table_);
        int new_capacity = next_prime(static_cast<int>(old_table.size()) * 2);
        table_.assign(new_capacity, Slot{});
        size_ = 0;
        for (const Slot& slot : old_table) {
            if (slot.state == State::Occupied) {
                put(slot.key, slot.value);
            }
        }
    }

    static int next_prime(int start) {
        for (int n = start; ; ++n) {
            if (is_prime(n)) return n;
        }
    }

    static bool is_prime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }
};
