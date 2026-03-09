/**
 * Demonstrates the HashTable class: insertion, retrieval, update, removal,
 * and automatic rehashing when the load factor exceeds 0.75.
 *
 * The table is initialised with a small capacity of 5 to make rehashing
 * easy to observe. In production, for a known-size dataset like the 118
 * confirmed chemical elements, an initial capacity of 150–200 would be
 * more appropriate to avoid unnecessary rehashing.
 *
 * Note: bucket placement will differ from the Java version because C++ and
 * Java use different string hash functions. The logical behaviour — which keys
 * are stored, retrieved, updated, and removed — is identical.
 */

#include <iostream>
#include <optional>

#include "hash_table.h"
#include "hash_function_type.h"

int main() {
    HashTable hash_table(5, HashFunctionType::Division);

    // Insert key-value pairs
    hash_table.put("Hydrogen", 1.008);
    hash_table.put("Helium",   4.0026);
    hash_table.put("Lithium",  6.94);

    std::cout << "Initial Hash Table:\n";
    hash_table.print();

    // Retrieve a specific value
    std::optional<double> helium_weight = hash_table.get("Helium");
    std::cout << "\nAtomic weight of Helium: " << helium_weight.value() << "\n";

    // Update an existing key
    hash_table.put("Helium", 4.002602);
    std::cout << "\nAfter updating Helium's atomic weight:\n";
    hash_table.print();

    // Remove an entry
    hash_table.remove("Lithium");
    std::cout << "\nAfter removing Lithium:\n";
    hash_table.print();

    // Insert more entries to trigger rehashing
    hash_table.put("Beryllium", 9.0122);
    hash_table.put("Boron",     10.81);
    hash_table.put("Carbon",    12.011);
    hash_table.put("Nitrogen",  14.007);
    hash_table.put("Oxygen",    15.999);

    std::cout << "\nAfter adding more elements and triggering rehashing:\n";
    hash_table.print();

    return 0;
}
