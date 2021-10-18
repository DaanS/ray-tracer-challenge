#include <gtest/gtest.h>
#include <numbers>
#include "transform.h"

TEST(Transform, Translation) {
    auto t = translation(5, -3, 2);
    auto p = point(-3, 4, 5);
    EXPECT_EQ(t * p, point(2, 1, 7));

    auto inv = inverse(t);
    EXPECT_EQ(inv * p, point(-8, 7, 3));

    auto v = vector(-3, 4, 5);
    EXPECT_EQ(t * v, v);
}

TEST(Transform, Scaling) {
    auto t = scaling(2, 3, 4);
    auto p = point(-4, 6, 8);
    EXPECT_EQ(t * p, point(-8, 18, 32));

    auto v = vector(-4, 6, 8);
    EXPECT_EQ(t * v, vector(-8, 18, 32));

    auto inv = inverse(t);
    EXPECT_EQ(inv * v, vector(-2, 2, 2));

    t = scaling(-1, 1, 1);
    p = point(2, 3, 4);
    EXPECT_EQ(t * p, point(-2, 3, 4));
}

using namespace std::numbers;

TEST(Transform, RotationX) {
    auto p = point(0, 1, 0);
    auto half_quarter = rotation_x(pi / 4);
    auto full_quarter = rotation_x(pi / 2);
    EXPECT_EQ(half_quarter * p, point(0, sqrt(2) / 2, sqrt(2) / 2));
    EXPECT_EQ(full_quarter * p, point(0, 0, 1));

    auto inv = inverse(half_quarter);
    EXPECT_EQ(inv * p, point(0, sqrt(2) / 2, -sqrt(2) / 2));
}

TEST(Transform, RotationY) {
    auto p = point(0, 0, 1);
    auto half_quarter = rotation_y(pi / 4);
    auto full_quarter = rotation_y(pi / 2);
    EXPECT_EQ(half_quarter * p, point(sqrt(2) / 2, 0, sqrt(2) / 2));
    EXPECT_EQ(full_quarter * p, point(1, 0, 0));
}

TEST(Transform, RotationZ) {
    auto p = point(0, 1, 0);
    auto half_quarter = rotation_z(pi / 4);
    auto full_quarter = rotation_z(pi / 2);
    EXPECT_EQ(half_quarter * p, point(-sqrt(2) / 2, sqrt(2) / 2, 0));
    EXPECT_EQ(full_quarter * p, point(-1, 0, 0));
}

TEST(Transform, Shear) {
    auto t = shearing(1, 0, 0, 0, 0, 0);
    auto p = point(2, 3, 4);
    EXPECT_EQ(t * p, point(5, 3, 4));

    t = shearing(0, 1, 0, 0, 0, 0);
    EXPECT_EQ(t * p, point(6, 3, 4));

    t = shearing(0, 0, 1, 0, 0, 0);
    EXPECT_EQ(t * p, point(2, 5, 4));

    t = shearing(0, 0, 0, 1, 0, 0);
    EXPECT_EQ(t * p, point(2, 7, 4));

    t = shearing(0, 0, 0, 0, 1, 0);
    EXPECT_EQ(t * p, point(2, 3, 6));

    t = shearing(0, 0, 0, 0, 0, 1);
    EXPECT_EQ(t * p, point(2, 3, 7));
}

TEST(Transform, Chain) {
    auto p = point(1, 0, 1);
    auto A = rotation_x(pi / 2);
    auto B = scaling(5, 5, 5);
    auto C = translation(10, 5, 7);

    auto p2 = A * p;
    EXPECT_EQ(p2, point(1, -1, 0));

    auto p3 = B * p2;
    EXPECT_EQ(p3, point(5, -5, 0));

    auto p4 = C * p3;
    EXPECT_EQ(p4, point(15, 0, 7));

    EXPECT_EQ(C * B * A * p, point(15, 0, 7));
}

TEST(Transform, DefaultView) {
    auto from = point(0, 0, 0);
    auto to = point(0, 0, -1);
    auto up = vector(0, 1, 0);
    auto t = view_transform(from, to, up);
    EXPECT_EQ(t, identity_matrix);
}

TEST(Transform, ViewZ) {
    auto from = point(0, 0, 0);
    auto to = point(0, 0, 1);
    auto up = vector(0, 1, 0);
    auto t = view_transform(from, to, up);
    EXPECT_EQ(t, scaling(-1, 1, -1));
}

TEST(Transform, ViewMove) {
    auto from = point(0, 0, 8);
    auto to = point(0, 0, 0);
    auto up = vector(0, 1, 0);
    auto t = view_transform(from, to, up);
    EXPECT_EQ(t, translation(0, 0, -8));
}

TEST(Transform, View) {
    auto from = point(1, 3, 2);
    auto to = point(4, -2, 8);
    auto up = vector(1, 1, 0);
    auto t = view_transform(from, to, up);
    EXPECT_EQ(t, matrix(
        -0.50709, 0.50709, 0.67612, -2.36643,
        0.76772, 0.60609, 0.12122, -2.82843,
        -0.35857, 0.59761, -0.71714, 0.00000,
        0.00000, 0.00000, 0.00000, 1.00000
    ));
}