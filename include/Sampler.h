#pragma once
#include <random>

inline double random_double() {
    // Returns a random_from_point real in [0,1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random_from_point real in [min,max).
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    // Returns a random_from_point integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}