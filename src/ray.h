#ifndef RAY_H
#define RAY_H

#include "tuple.h"
#include "matrix.h"

struct ray {
    struct tuple origin;
    struct tuple direction;
};

struct ray ray(struct tuple origin, struct tuple direction) {
    return {origin, direction};
}

struct tuple position(struct ray const& r, double t) {
    return r.origin + r.direction * t;
}

struct ray transform(struct ray const& r, struct matrix_t<4> const& m) {
    return {m * r.origin, m * r.direction};
}

#endif