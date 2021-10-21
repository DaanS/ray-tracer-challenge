#ifndef UTIL_H
#define UTIL_H

#include <cmath>

static constexpr double epsilon{0.0001};
static const double half_sqrt_2{std::sqrt(2) / 2};

bool compare(double a, double b) {
    return std::abs(a - b) < epsilon;
}

#endif