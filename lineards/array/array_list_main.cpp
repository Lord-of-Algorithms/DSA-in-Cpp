/**
 * Demonstrates the use of a dynamic array (std::vector).
 * Showcases various operations such as insertion and deletion
 * in a dynamically sized array.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void print_list(const std::vector<std::string>& list) {
    std::cout << "Current state of the list:\n[";
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        std::cout << list[i];
        if (i + 1 < static_cast<int>(list.size())) std::cout << ", ";
    }
    std::cout << "]\n\n";
}

int main() {
    // 1. Initialization with initial values
    std::vector<std::string> planets;

    planets.push_back("Venus");
    planets.push_back("Pluto");
    planets.push_back("Earth");
    std::cout << "Initial values...\n";
    print_list(planets);

    // 2. Insertion at the end
    planets.push_back("Jupiter");
    std::cout << "Adding 'Jupiter' at the end...\n";
    print_list(planets);

    // 3. Insertion at the beginning
    planets.insert(planets.begin(), "Mercury");
    std::cout << "Inserting 'Mercury' at the beginning...\n";
    print_list(planets);

    // 4. Insertion at a specified position
    planets.insert(planets.begin() + 4, "Mars");
    std::cout << "Inserting 'Mars' at position 4...\n";
    print_list(planets);

    // 5. Deletion by index
    planets.erase(planets.begin() + 2);
    std::cout << "Removing 'Pluto' at position 2...\n";
    print_list(planets);

    // 6. Deletion by value
    auto it = std::find(planets.begin(), planets.end(), "Jupiter");
    if (it != planets.end()) planets.erase(it);
    std::cout << "Removing 'Jupiter' by value...\n";
    print_list(planets);

    return 0;
}
