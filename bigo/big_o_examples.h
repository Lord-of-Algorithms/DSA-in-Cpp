#pragma once

#include <iostream>
#include <string>
#include <vector>

/**
 * One function per growth class of the Big O topic.
 *
 * Big O describes how the number of steps grows with the size of the
 * input, n. Counting how many times a function's loops can run, or how
 * many times it calls itself, gives its class:
 *
 *   * read_at       - no loop, one step:             O(1)
 *   * binary_search - a loop that halves its range:  O(log n)
 *   * linear_search - one loop over the n values:    O(n)
 *   * has_duplicate - a loop inside a loop:          O(n^2)
 *   * print_coins   - every combination of n coins: O(2^n)
 *
 * Each class is given for the worst case.
 */

// O(1): reads the value at the given index in one step, whatever the
// length of the vector.
inline int read_at(const std::vector<int>& values, int index) {
    return values[index];
}

// O(n): looks at the values one by one from the left. Returns the index of
// the target, or -1 if it is missing. In the worst case (the target is last
// or missing) the loop runs n times.
inline int linear_search(const std::vector<int>& values, int target) {
    int n = static_cast<int>(values.size());
    for (int i = 0; i < n; i++) {
        if (values[i] == target) {
            return i;
        }
    }
    return -1;
}

// O(log n): the values must be sorted in ascending order. Each look at the
// middle of the remaining range rules out half of it, so the loop runs about
// log2(n) + 1 times at most. Returns the index of the target, or -1 if it is
// missing.
inline int binary_search(const std::vector<int>& values, int target) {
    int lo = 0;
    int hi = static_cast<int>(values.size()) - 1;
    while (lo <= hi) {
        // The left middle when the range has two; this form of
        // (lo + hi) / 2 cannot overflow.
        int mid = lo + (hi - lo) / 2;
        if (values[mid] == target) {
            return mid;
        } else if (values[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

// O(n^2): compares every pair of values, n(n - 1) / 2 comparisons when there
// is no duplicate. Returns true as soon as two values are equal.
inline bool has_duplicate(const std::vector<int>& values) {
    int n = static_cast<int>(values.size());
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (values[i] == values[j]) {
                return true;
            }
        }
    }
    return false;
}

// Coins before index i are set; tries coin i both ways.
inline void print_all(std::string& coins, int i) {
    if (i == static_cast<int>(coins.size())) {
        std::cout << coins << "\n";
        return;
    }
    coins[i] = 'H';
    print_all(coins, i + 1);
    coins[i] = 'T';
    print_all(coins, i + 1);
}

// O(2^n): prints every way n coins can land, one line each, heads (H) or
// tails (T). Each coin doubles the number of combinations, so there are 2^n
// lines. Uses recursion: print_all calls itself.
inline void print_coins(int n) {
    std::string coins(n, 'H');
    print_all(coins, 0);
}
