#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

struct matrix_t<4> translation(double x, double y, double z) {
    return {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };
}

struct matrix_t<4> scaling(double x, double y, double z) {
    return {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

struct matrix_t<4> rotation_x(double r) {
    return {
        1, 0, 0, 0,
        0, std::cos(r), -std::sin(r), 0,
        0, std::sin(r), std::cos(r), 0,
        0, 0, 0, 1
    };
}

struct matrix_t<4> rotation_y(double r) {
    return {
        std::cos(r), 0, std::sin(r), 0,
        0, 1, 0, 0,
        -std::sin(r), 0, std::cos(r), 0,
        0, 0, 0, 1
    };
}

struct matrix_t<4> rotation_z(double r) {
    return {
        std::cos(r), -std::sin(r), 0, 0,
        std::sin(r), std::cos(r), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

struct matrix_t<4> shearing(double xy, double xz, double yx, double yz, double zx, double zy) {
    return {
        1, xy, xz, 0,
        yx, 1, yz, 0,
        zx, zy, 1, 0,
        0, 0, 0, 1
    };
}
#endif