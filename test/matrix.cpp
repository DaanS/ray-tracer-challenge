#include <gtest/gtest.h>
#include "matrix.h"

TEST(Matrix, Create4) {
    auto m = matrix(
        1, 2, 3, 4,
        5.5, 6.5, 7.5, 8.5,
        9, 10, 11, 12,
        13.5, 14.5, 15.5, 15.5
    );
    EXPECT_EQ(m(0, 0), 1);
    EXPECT_EQ(m(0, 3), 4);
    EXPECT_EQ(m(1, 0), 5.5);
    EXPECT_EQ(m(1, 2), 7.5);
    EXPECT_EQ(m(2, 2), 11);
    EXPECT_EQ(m(3, 0), 13.5);
    EXPECT_EQ(m(3, 2), 15.5);
}

TEST(Matrix, Create3) {
    auto m = matrix(
        -3, 5, 0,
        1, -2, -7,
        0, 1, 1
    );
    EXPECT_EQ(m(0, 0), -3);
    EXPECT_EQ(m(1, 1), -2);
    EXPECT_EQ(m(2, 2), 1);
}

TEST(Matrix, Create2) {
    auto m = matrix(
        -3, 5,
        1, -2
    );
    EXPECT_EQ(m(0, 1), 5);
    EXPECT_EQ(m(1, 0), 1);
    EXPECT_EQ(m(1, 1), -2);
}

TEST(Matrix, Equality) {
    auto m1 = matrix(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    );
    auto m2 = matrix(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    );
    auto m3 = matrix(
        2, 3, 4, 5,
        6, 7, 8, 9,
        8, 7, 6, 5,
        4, 3, 2, 1
    );
    EXPECT_EQ(m1, m2);
    EXPECT_NE(m1, m3);
}

TEST(Matrix, Multiplication) {
    auto m1 = matrix(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    );
    auto m2 = matrix(
        -2, 1, 2, 3,
        3, 2, 1, -1,
        4, 3, 6, 5,
        1, 2, 7, 8
    );
    EXPECT_EQ(m1 * m2, matrix(
        20, 22, 50, 48,
        44, 54, 114, 108,
        40, 58, 110, 102,
        16, 26, 46, 42
    ));
}

TEST(Matrix, TupleMultiplication) {
    auto A = matrix(
        1, 2, 3, 4,
        2, 4, 4, 2,
        8, 6, 4, 1,
        0, 0, 0, 1
    );
    auto b = tuple(1, 2, 3, 1);
    EXPECT_EQ(A * b, tuple(18, 24, 33, 1));
}

TEST(Matrix, Identity) {
    auto A = matrix(
        0, 1, 2, 4,
        1, 2, 4, 8,
        2, 4, 8, 16,
        4, 8, 16, 32
    );
    auto I = identity_matrix;
    auto a = tuple(1, 2, 3, 4);
    EXPECT_EQ(I * A, A);
    EXPECT_EQ(I * a, a);
}

TEST(Matrix, Transpose) {
    auto A = matrix(
        0, 9, 3, 0,
        9, 8, 0, 8,
        1, 8, 5, 3,
        0, 0, 5, 5
    );
    EXPECT_EQ(transpose(A), matrix(
        0, 9, 1, 0,
        9, 8, 8, 0,
        3, 0, 5, 5,
        0, 8, 3, 5
    ));
}

TEST(Matrix, Submatrix3) {
    auto A = matrix(
        1, 5, 0,
        -3, 2, 7,
        0, 6, -3
    );
    EXPECT_EQ(submatrix(A, 0, 2), matrix(
        -3, 2,
        0, 6
    ));
}

TEST(Matrix, Submatrix4) {
    auto A = matrix(
        -6, 1, 1, 6,
        -8, 5, 8, 6,
        -1, 0, 8, 2,
        -7, 1, -1, 1
    );
    EXPECT_EQ(submatrix(A, 2, 1), matrix(
        -6, 1, 6,
        -8, 8, 6,
        -7, -1, 1
    ));
}

TEST(Matrix, Minor3) {
    auto A = matrix(
        3, 5, 0,
        2, -1, -7,
        6, -1, 5
    );
    EXPECT_EQ(determinant(submatrix(A, 1, 0)), 25);
    EXPECT_EQ(minor(A, 1, 0), 25);
}

TEST(Matrix, Cofactor3) {
    auto A = matrix(
        3, 5, 0,
        2, -1, -7,
        6, -1, 5
    );
    EXPECT_EQ(minor(A, 0, 0), -12);
    EXPECT_EQ(cofactor(A, 0, 0), -12);
    EXPECT_EQ(minor(A, 1, 0), 25);
    EXPECT_EQ(cofactor(A, 1, 0), -25);
}

TEST(Matrix, Determinant2) {
    auto A = matrix(
        1, 5,
        -3, 2
    );
    EXPECT_EQ(determinant(A), 17);
}

TEST(Matrix, Determinant3) {
    auto A = matrix(
        1, 2, 6,
        -5, 8, -4,
        2, 6, 4
    );
    EXPECT_EQ(cofactor(A, 0, 0), 56);
    EXPECT_EQ(cofactor(A, 0, 1), 12);
    EXPECT_EQ(cofactor(A, 0, 2), -46);
    EXPECT_EQ(determinant(A), -196);
}

TEST(Matrix, Determinant4) {
    auto A = matrix(
        -2, -8, 3, 5,
        -3, 1, 7, 3,
        1, 2, -9, 6,
        -6, 7, 7, -9
    );
    EXPECT_EQ(cofactor(A, 0, 0), 690);
    EXPECT_EQ(cofactor(A, 0, 1), 447);
    EXPECT_EQ(cofactor(A, 0, 2), 210);
    EXPECT_EQ(cofactor(A, 0, 3), 51);
    EXPECT_EQ(determinant(A), -4071);
}

TEST(Matrix, IsInvertible) {
    auto A = matrix(
        6, 4, 4, 4,
        5, 5, 7, 6,
        4, -9, 3, -7,
        9, 1, 7, -6
    );
    EXPECT_EQ(determinant(A), -2120);
    EXPECT_TRUE(is_invertible(A));

    auto B = matrix(
        -4, 2, -2, -3,
        9, 6, 2, 6,
        0, -5, 1, -5,
        0, 0, 0, 0
    );
    EXPECT_EQ(determinant(B), 0);
    EXPECT_FALSE(is_invertible(B));
}

TEST(Matrix, Inverse) {
    auto A = matrix(
        -5, 2, 6, -8,
        1, -5, 1, 8,
        7, 7, -6, -7,
        1, -3, 7, 4
    );
    auto B = inverse(A);
    EXPECT_EQ(determinant(A), 532);
    EXPECT_EQ(cofactor(A, 2, 3), -160);
    EXPECT_EQ(B(3, 2), -160.0/532);
    EXPECT_EQ(cofactor(A, 3, 2), 105);
    EXPECT_EQ(B(2, 3), 105.0/532);
    EXPECT_EQ(B, matrix(
         0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361, 0.52068,
        -0.07895, -0.22368, -0.05263, 0.19737,
        -0.52256, -0.81391, -0.30075, 0.30639

    ));

    A = matrix(
        8, -5, 9, 2,
        7, 5, 6, 1,
        -6, 0, 9, 6,
        -3, 0, -9, -4
    );
    EXPECT_EQ(inverse(A), matrix(
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692, 0.12308, 0.02564, 0.03077,
        0.35897, 0.35897, 0.43590, 0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308
    ));

    A = matrix(
        9, 3, 0, 9,
        -5, -2, -6, -3,
        -4, 9, 6, 4,
        -7, 6, 6, 2
    );
    EXPECT_EQ(inverse(A), matrix(
        -0.04074, -0.07778, 0.14444, -0.22222,
        -0.07778, 0.03333, 0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926, 0.12963,
        0.17778, 0.06667, -0.26667, 0.33333
    ));
}

TEST(Matrix, InverseMultiplication) {
    auto A = matrix(
        3, -9, 7, 3,
        3, -8, 2, -9,
        -4, 4, 4, 1,
        -6, 5, -1, 1
    );
    auto B = matrix(
        8, 2, 2, 2,
        3, -1, 7, 0,
        7, 0, 5, 4,
        6, -2, 0, 5
    );
    auto C = A * B;
    EXPECT_EQ(C * inverse(B), A);
}