#pragma once

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "hash_function_type.h"

/**
 * A hash table implementation using chaining with linked lists to resolve collisions.
 * Supports inserting, retrieving, and removing string-keyed, double-valued pairs.
 *
 * The table resizes automatically when the load factor exceeds 0.75: the capacity
 * is doubled and rounded up to the next prime, then all entries are rehashed.
 * Two hash functions are available (Division and Multiplication) and are chosen
 * at construction time via HashFunctionType.
 */
class HashTable {
public:
    HashTable(int capacity, HashFunctionType hash_function_type)
        : size_(0), hash_function_type_(hash_function_type) {
        if (capacity < 1) {
            throw std::invalid_argument("Initial capacity must be >= 1.");
        }
        // Adjust capacity to the nearest prime for better distribution
        capacity = is_prime(capacity) ? capacity : next_prime(capacity);
        bucket_array_.assign(capacity, nullptr);
    }

    ~HashTable() {
        for (auto* bucket : bucket_array_) delete bucket;
    }

    /**
     * Inserts or updates a key-value pair.
     * Triggers rehashing if adding the entry would exceed the load factor.
     */
    void put(const std::string& key, double value) {
        int index = hash(key);
        if (!bucket_array_[index]) {
            bucket_array_[index] = new HashTableBucket();
        }

        HashTableBucket* bucket = bucket_array_[index];
        HashTableEntry* existing = bucket->find(key);
        if (existing) {
            existing->value = value;  // Key already present — update value
        } else {
            // Check load factor before inserting; rehash if necessary
            if (static_cast<double>(size_ + 1) / static_cast<double>(bucket_array_.size()) >= MAX_LOAD_FACTOR) {
                rehash();
                index = hash(key);  // Recompute index after rehash
                if (!bucket_array_[index]) {
                    bucket_array_[index] = new HashTableBucket();
                }
                bucket = bucket_array_[index];
            }
            bucket->insert_at_beginning(key, value);
            ++size_;
        }
    }

    /**
     * Returns the value associated with the key, or nullopt if not found.
     */
    std::optional<double> get(const std::string& key) const {
        int index = hash(key);
        const HashTableBucket* bucket = bucket_array_[index];
        if (!bucket) return std::nullopt;
        const HashTableEntry* entry = bucket->find(key);
        return entry ? std::optional<double>(entry->value) : std::nullopt;
    }

    /**
     * Removes the entry with the given key.
     * @return true if the key was found and removed, false otherwise.
     */
    bool remove(const std::string& key) {
        int index = hash(key);
        if (!bucket_array_[index]) return false;
        if (bucket_array_[index]->delete_entry(key)) {
            --size_;
            return true;
        }
        return false;
    }

    int size() const { return size_; }

    /**
     * Prints the entire contents of the hash table, bucket by bucket.
     */
    void print() const {
        for (int i = 0; i < static_cast<int>(bucket_array_.size()); ++i) {
            std::cout << "[" << i << "]";
            if (bucket_array_[i]) bucket_array_[i]->print();
            std::cout << "\n";
        }
    }

private:
    // Knuth's multiplicative constant: (sqrt(5) - 1) / 2
    static constexpr double A = 0.6180339887498949;
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    // -------------------------------------------------------------------------
    // HashTableEntry — a node in the chaining linked list
    // -------------------------------------------------------------------------
    struct HashTableEntry {
        std::string     key;
        double          value;
        HashTableEntry* next = nullptr;

        HashTableEntry(std::string k, double v)
            : key(std::move(k)), value(v) {}

        void print() const {
            // Use enough precision to match Java's Double.toString() behaviour,
            // which always shows the minimum digits needed to uniquely represent the value.
            std::cout << "->|" << key << ", "
                      << std::setprecision(10) << value
                      << std::setprecision(6)  // restore default
                      << "|";
        }
    };

    // -------------------------------------------------------------------------
    // HashTableBucket — a singly linked list (one per array slot)
    // -------------------------------------------------------------------------
    struct HashTableBucket {
        HashTableEntry* head = nullptr;

        ~HashTableBucket() {
            HashTableEntry* current = head;
            while (current) {
                HashTableEntry* next = current->next;
                delete current;
                current = next;
            }
        }

        // Inserts a new entry at the front of the list
        void insert_at_beginning(const std::string& key, double value) {
            HashTableEntry* entry = new HashTableEntry(key, value);
            entry->next = head;
            head = entry;
        }

        /**
         * Removes the entry with the given key.
         * @return true if found and deleted, false otherwise.
         */
        bool delete_entry(const std::string& key) {
            HashTableEntry* current = head;
            HashTableEntry* prev    = nullptr;
            while (current) {
                if (current->key == key) {
                    if (!prev) head = current->next;  // Deleting the first entry
                    else       prev->next = current->next;
                    delete current;
                    return true;
                }
                prev    = current;
                current = current->next;
            }
            return false;
        }

        HashTableEntry* find(const std::string& key) const {
            HashTableEntry* current = head;
            while (current) {
                if (current->key == key) return current;
                current = current->next;
            }
            return nullptr;
        }

        void print() const {
            HashTableEntry* current = head;
            while (current) {
                current->print();
                current = current->next;
            }
        }
    };

    // -------------------------------------------------------------------------
    // HashTable internals
    // -------------------------------------------------------------------------
    std::vector<HashTableBucket*> bucket_array_;
    int              size_;
    HashFunctionType hash_function_type_;

    int hash(const std::string& key) const {
        // std::hash<std::string> returns size_t (unsigned) — no need for abs()
        std::size_t hash_code = std::hash<std::string>{}(key);
        int m = static_cast<int>(bucket_array_.size());

        switch (hash_function_type_) {
            case HashFunctionType::Division:
                return static_cast<int>(hash_code % (std::size_t)m);

            case HashFunctionType::Multiplication: {
                double fractional = std::fmod(hash_code * A, 1.0);
                return static_cast<int>(std::floor(m * fractional));
            }

            default:
                throw std::invalid_argument("Unknown HashFunctionType.");
        }
    }

    // Doubles the capacity, rounds up to next prime, and rehashes all entries
    void rehash() {
        std::vector<HashTableBucket*> old_buckets = std::move(bucket_array_);
        int new_capacity = next_prime(static_cast<int>(old_buckets.size()) * 2);
        bucket_array_.assign(new_capacity, nullptr);
        size_ = 0;

        for (auto* bucket : old_buckets) {
            if (bucket) {
                HashTableEntry* current = bucket->head;
                while (current) {
                    put(current->key, current->value);
                    current = current->next;
                }
                delete bucket;
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
