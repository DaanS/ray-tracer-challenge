#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "util.h"
#include "tuple.h"

template<size_t size>
struct matrix_t {
    std::array<double, size * size> m;

    double& operator()(int y, int x) {
        return m[x + size * y];
    }

    double const& operator()(int y, int x) const {
        return m[x + size * y];
    }

    bool operator==(struct matrix_t<size> const& rhs) const {
        bool equal = true;
        for (int i = 0; i < m.size(); ++i) {
            equal = equal && compare(m[i], rhs.m[i]);
        }
        return equal;
    }

    struct matrix_t<size> operator*(struct matrix_t<size> const& rhs) const {
        struct matrix_t<size> res;
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                res(y, x) = (*this)(y, 0) * rhs(0, x) +
                            (*this)(y, 1) * rhs(1, x) +
                            (*this)(y, 2) * rhs(2, x) +
                            (*this)(y, 3) * rhs(3, x);
            }
        }
        return res;
    }

    struct tuple operator*(struct tuple t) const {
        return {
            m[0] * t.x + m[1] * t.y + m[2] * t.z + m[3] * t.w,
            m[4] * t.x + m[5] * t.y + m[6] * t.z + m[7] * t.w,
            m[8] * t.x + m[9] * t.y + m[10] * t.z + m[11] * t.w,
            m[12] * t.x + m[13] * t.y + m[14] * t.z + m[15] * t.w,
        };
    }

    friend std::ostream& operator<<(std::ostream& os, struct matrix_t<size> const& m) {
        os << "{";
        for (int i = 0; i < size * size - 1; ++i) os << m.m[i] << ", ";
        os << m.m[size * size - 1];
        os << "}\n";
        return os;
    }
};

static constexpr struct matrix_t<4> identity_matrix{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

struct matrix_t<4> matrix(double a00, double a01, double a02, double a03,
                          double a10, double a11, double a12, double a13,
                          double a20, double a21, double a22, double a23,
                          double a30, double a31, double a32, double a33) 
{
    return {a00, a01, a02, a03,
            a10, a11, a12, a13,
            a20, a21, a22, a23,
            a30, a31, a32, a33};
}

struct matrix_t<3> matrix(double a00, double a01, double a02,
                          double a10, double a11, double a12,
                          double a20, double a21, double a22)
{
    return {a00, a01, a02,
            a10, a11, a12,
            a20, a21, a22};
}

struct matrix_t<2> matrix(double a00, double a01,
                          double a10, double a11)
{
    return {a00, a01,
            a10, a11};
}

template<size_t size>
struct matrix_t<size> transpose(struct matrix_t<size> const& m) {
    struct matrix_t<size> res;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            res(y, x) = m(x, y);
        }
    }
    return res;
}

template<size_t size>
struct matrix_t<size - 1> submatrix(struct matrix_t<size> const& m, int row, int col) {
    struct matrix_t<size - 1> res;
    int res_y = 0;
    for (int y = 0; y < size; ++y) {
        if (y == row) continue;
        int res_x = 0;
        for (int x = 0; x < size; ++x) {
            if (x == col) continue;
            res(res_y, res_x) = m(y, x);
            ++res_x;
        }
        ++res_y;
    }
    return res;
}

template<size_t size>
double minor(struct matrix_t<size> const& m, int y, int x);

template<size_t size>
double cofactor(struct matrix_t<size> const& m, int y, int x) {
    return minor(m, y, x) * ((y + x) % 2 == 0 ? 1 : -1);
}

template<size_t size>
double determinant(struct matrix_t<size> const& m) {
    double res = 0;
    for (int x = 0; x < size; ++x) {
        res += m(0, x) * cofactor(m, 0, x);
    }
    return res;
}

template<>
double determinant(struct matrix_t<2> const& m) {
    return m.m[0] * m.m[3] - m.m[1] * m.m[2];
}

template<size_t size>
double minor(struct matrix_t<size> const& m, int y, int x) {
    return determinant(submatrix(m, y, x));
}

template<size_t size>
bool is_invertible(struct matrix_t<size> const& m) {
    return determinant(m) != 0;
}

template<size_t size>
struct matrix_t<size> inverse(struct matrix_t<size> const& m) {
    assert(is_invertible(m));
    struct matrix_t<size> res;
    double det = determinant(m);
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            res(x, y) = cofactor(m, y, x) / det;
        }
    }
    return res;
}

#endif