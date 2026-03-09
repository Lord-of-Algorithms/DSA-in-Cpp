#pragma once

/**
 * Enumerates the supported hash function types.
 */
enum class HashFunctionType {
    Division,       // h(k) = k mod m
    Multiplication  // h(k) = floor(m * ((k * A) mod 1)), where A = (sqrt(5) - 1) / 2
};
