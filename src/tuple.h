#ifndef TUPLE_H
#define TUPLE_H

#include <cmath>
#include "util.h"

//bool compare(struct tuple lhs, struct tuple rhs, double e) {
//    return compare(lhs.x, rhs.x, e) &&
//           compare(lhs.y, rhs.y, e) &&
//           compare(lhs.z, rhs.z, e) &&
//           compare(lhs.w, rhs.w, e);
//}

struct tuple {
    union {double x; double r;};
    union {double y; double g;}; 
    union {double z; double b;}; 
    double w;

    bool is_point() const {
        return w == 1.0;
    }

    bool is_vector() const {
        return w == 0.0;
    }

    bool operator==(struct tuple rhs) const {
        return compare(x, rhs.x) &&
               compare(y, rhs.y) &&
               compare(z, rhs.z) &&
               compare(w, rhs.w);
    }

    struct tuple operator+(struct tuple rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
    }

    struct tuple operator-(struct tuple rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
    }

    struct tuple operator-() const {
        return {-x, -y, -z, -w};
    }

    struct tuple operator*(double a) const {
        return {x * a, y * a, z * a, w * a};
    }

    struct tuple operator*(struct tuple rhs) const {
        return {x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w};
    }

    struct tuple operator/(double a) const {
        return {x / a, y / a, z / a, w / a};
    }

    void operator+=(struct tuple rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
    }

    friend std::ostream& operator<<(std::ostream& os, struct tuple a) {
        os << "{" << a.x << ", " << a.y << ", " << a.z << ", " << a.w << "}";
        return os;
    }
};

struct tuple tuple(double x, double y, double z, double w) {
    return {x, y, z, w};
}

struct tuple point(double x, double y, double z) {
    return {x, y, z, 1};
}

struct tuple vector(double x, double y, double z) {
    return {x, y, z, 0};
}

struct tuple color(double r, double g, double b) {
    return {r, g, b, 0};
}

double magnitude(struct tuple a) {
    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

struct tuple normalize(struct tuple a) {
    auto m = magnitude(a);
    return {a.x / m, a.y / m, a.z / m, a.w / m};
}

double dot(struct tuple a, struct tuple b) {
    return a.x * b.x +
           a.y * b.y + 
           a.z * b.z + 
           a.w * b.w;
}

struct tuple cross(struct tuple a, struct tuple b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

struct tuple reflect(struct tuple in, struct tuple normal) {
    return in - normal * 2 * dot(in, normal);
}

#endif