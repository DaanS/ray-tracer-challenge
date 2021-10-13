#ifndef UTIL_H
#define UTIL_H

static constexpr double e{0.0001};

bool compare(double a, double b) {
    return std::abs(a - b) < e;
}

#endif