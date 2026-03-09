/**
 * Demonstrates the operations on a static array.
 * Showcases insertion at specific positions, deletion, and basic
 * array handling in a fixed-size array structure.
 */

#include <iostream>
#include <string>

static constexpr int CAPACITY = 8;

// Prints the contents of a fixed-size array, showing "null" for empty slots.
void print_array(const std::string arr[], int length) {
    std::cout << "Current state of the array:\n";
    for (int i = 0; i < length; ++i) {
        std::cout << (arr[i].empty() ? "null" : arr[i]) << " ";
    }
    std::cout << "\n\n";
}

int main() {
    // 1. Initialization: fixed-size array of 8 strings (empty string = "null")
    std::string planets[CAPACITY];

    // 2. Assigning Values
    planets[0] = "Venus";
    planets[1] = "Pluto";
    planets[2] = "Earth";
    planets[3] = "Jupiter";
    std::cout << "Assigning initial values...\n";
    print_array(planets, CAPACITY);

    // 3. Insertion at the beginning
    // Shift elements to the right to make space for the new element
    for (int i = CAPACITY - 1; i > 0; --i) {
        planets[i] = planets[i - 1];
    }
    planets[0] = "Mercury";
    std::cout << "Inserting 'Mercury' at the beginning...\n";
    print_array(planets, CAPACITY);

    // 4. Insertion at an arbitrary position
    int position = 4;
    for (int i = CAPACITY - 1; i > position; --i) {
        planets[i] = planets[i - 1];
    }
    planets[position] = "Mars";
    std::cout << "Inserting 'Mars' at position " << position << "...\n";
    print_array(planets, CAPACITY);

    // 5. Deletion at an arbitrary position
    position = 2;
    for (int i = position; i < CAPACITY - 1; ++i) {
        planets[i] = planets[i + 1];
    }
    planets[CAPACITY - 1] = "";  // Clear the last element
    std::cout << "Deleting element at position " << position << "...\n";
    print_array(planets, CAPACITY);

    return 0;
}
