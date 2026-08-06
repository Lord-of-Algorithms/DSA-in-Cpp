#pragma once

/**
 * Enumerates the supported probe sequences for open addressing.
 */
enum class ProbeType {
    Linear,     // offset(i) = i        -> home, home+1, home+2, ...
    Quadratic   // offset(i) = i * i    -> home, home+1, home+4, home+9, ...
};
