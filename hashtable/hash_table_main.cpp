/**
 * Demonstrates both hash table implementations on the same kind of data — a
 * small phone book that maps names (keys) to phone numbers (values):
 *
 *   1. Chaining        — collisions share a slot via a linked list.
 *   2. Open addressing — collisions are resolved by probing for another slot,
 *                        with tombstones marking removed entries.
 */

#include <iostream>

#include "chaining/chaining_hash_table.h"
#include "chaining/hash_function_type.h"
#include "openaddressing/open_addressing_hash_table.h"
#include "openaddressing/probe_type.h"

static void demo_chaining() {
    std::cout << "=== Chaining ===\n";
    ChainingHashTable phone_book(5, HashFunctionType::Division);

    // Insert name -> phone number pairs.
    phone_book.put("Alice", "555-0101");
    phone_book.put("Bob",   "555-0102");
    phone_book.put("Carol", "555-0103");

    std::cout << "Initial phone book:\n";
    phone_book.print();

    // Retrieve a value.
    std::cout << "\nBob's number: " << phone_book.get("Bob").value() << "\n";

    // Update an existing key.
    phone_book.put("Bob", "555-9999");
    std::cout << "\nAfter updating Bob's number:\n";
    phone_book.print();

    // Remove an entry.
    phone_book.remove("Carol");
    std::cout << "\nAfter removing Carol:\n";
    phone_book.print();

    // Insert more entries to trigger rehashing.
    phone_book.put("Dave",  "555-0104");
    phone_book.put("Eve",   "555-0105");
    phone_book.put("Frank", "555-0106");
    std::cout << "\nAfter adding more names and triggering rehashing:\n";
    phone_book.print();
}

static void demo_open_addressing() {
    std::cout << "=== Open addressing ===\n";
    OpenAddressingHashTable phone_book(7, ProbeType::Linear);

    // Insert name -> phone number pairs.
    phone_book.put("Bob", "555-0102");
    phone_book.put("Rob", "555-0103");
    phone_book.put("Tam", "555-0104");

    std::cout << "After inserts (linear probing):\n";
    phone_book.print();

    // Retrieve a value.
    std::cout << "\nRob's number: " << phone_book.get("Rob").value() << "\n";

    // Remove a key — leaves a tombstone so probe chains stay intact.
    phone_book.remove("Rob");
    std::cout << "\nAfter removing Rob:\n";
    phone_book.print();

    // Searching for the removed key now fails; keys past the tombstone are still found.
    std::cout << "\nRob found after removal: " << (phone_book.get("Rob").has_value() ? "yes" : "no") << "\n";
    std::cout << "Tam found after removal: " << (phone_book.get("Tam").has_value() ? "yes" : "no") << "\n";

    // A new insert can reuse the tombstoned slot.
    phone_book.put("Max", "555-0105");
    std::cout << "\nAfter inserting Max (may reuse the tombstone):\n";
    phone_book.print();

    // The same operations with quadratic probing.
    OpenAddressingHashTable quad_book(7, ProbeType::Quadratic);
    quad_book.put("Bob", "555-0102");
    quad_book.put("Rob", "555-0103");
    quad_book.put("Vic", "555-0104");
    std::cout << "\nAfter inserts (quadratic probing):\n";
    quad_book.print();
}

int main() {
    demo_chaining();
    std::cout << "\n";
    demo_open_addressing();
    return 0;
}
