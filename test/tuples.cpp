#include <gtest/gtest.h>
#include "tuple.h"

TEST(Tuples, TupleIsPoint) {
    auto a = tuple(4.3, -4.2, 3.1, 1.0);
    EXPECT_EQ(a.x, 4.3);
    EXPECT_EQ(a.y, -4.2);
    EXPECT_EQ(a.z, 3.1);
    EXPECT_EQ(a.w, 1.0);
    EXPECT_TRUE(a.is_point());
    EXPECT_FALSE(a.is_vector());
}

TEST(Tuples, TupleIsVector) {
    auto a = tuple(4.3, -4.2, 3.1, 0.0);
    EXPECT_EQ(a.x, 4.3);
    EXPECT_EQ(a.y, -4.2);
    EXPECT_EQ(a.z, 3.1);
    EXPECT_EQ(a.w, 0.0);
    EXPECT_FALSE(a.is_point());
    EXPECT_TRUE(a.is_vector());
}

TEST(Tuples, Point) {
    auto p = point(4, -4, 3);
    EXPECT_EQ(p, tuple(4, -4, 3, 1));
}

TEST(Tuples, Vector) {
    auto v = vector(4, -4, 3);
    EXPECT_EQ(v, tuple(4, -4, 3, 0));
}

TEST(Tuples, Addition) {
    auto a1 = tuple(3, -2, 5, 1);
    auto a2 = tuple(-2, 3, 1, 0);
    EXPECT_EQ(a1 + a2, tuple(1, 1, 6, 1));
}

TEST(Tuples, InPlaceAddition) {
    auto a1 = tuple(3, -2, 5, 1);
    a1 += tuple(-2, 3, 1, 0);
    EXPECT_EQ(a1, tuple(1, 1, 6, 1));
}

TEST(Tuples, SubtractPoints) {
    auto p1 = point(3, 2, 1);
    auto p2 = point(5, 6, 7);
    EXPECT_EQ(p1 - p2, vector(-2, -4, -6));
}

TEST(Tuples, SubtractVectorFromPoint) {
    auto p = point(3, 2, 1);
    auto v = vector(5, 6, 7);
    EXPECT_EQ(p - v, point(-2, -4, -6));
}

TEST(Tuples, SubtractVectors) {
    auto v1 = vector(3, 2, 1);
    auto v2 = vector(5, 6, 7);
    EXPECT_EQ(v1 - v2, vector(-2, -4, -6));
}

TEST(Tuples, SubtractVectorFromZero) {
    auto zero = vector(0, 0, 0);
    auto v = vector(1, -2, 3);
    EXPECT_EQ(zero - v, vector(-1, 2, -3));
}

TEST(Tuples, Negate) {
    auto a = tuple(1, -2, 3, -4);
    EXPECT_EQ(-a, tuple(-1, 2, -3, 4));
}

TEST(Tuples, MultiplicationScalar) {
    auto a = tuple(1, -2, 3, -4);
    EXPECT_EQ(a * 3.5, tuple(3.5, -7, 10.5, -14));
}

TEST(Tuples, MultiplicationFraction) {
    auto a = tuple(1, -2, 3, -4);
    EXPECT_EQ(a * 0.5, tuple(0.5, -1, 1.5, -2));
}

TEST(Tuples, DivisionScalar) {
    auto a = tuple(1, -2, 3, -4);
    EXPECT_EQ(a / 2, tuple(0.5, -1, 1.5, -2));
}

TEST(Tuples, Magnitude) {
    auto v = vector(1, 0, 0);
    EXPECT_EQ(magnitude(v), 1);

    v = vector(0, 1, 0);
    EXPECT_EQ(magnitude(v), 1);

    v = vector(0, 0, 1);
    EXPECT_EQ(magnitude(v), 1);

    v = vector(1, 2, 3);
    EXPECT_EQ(magnitude(v), std::sqrt(14));

    v = vector(-1, -2, -3);
    EXPECT_EQ(magnitude(v), std::sqrt(14));
}

TEST(Tuples, Normalize) {
    auto v = vector(4, 0, 0);
    EXPECT_EQ(normalize(v), vector(1, 0, 0));

    v = vector(1, 2, 3);
    EXPECT_EQ(normalize(v), vector(1 / std::sqrt(14), 2 / std::sqrt(14), 3 / std::sqrt(14)));

    auto norm = normalize(v);
    EXPECT_EQ(magnitude(norm), 1);
}

TEST(Tuples, Dot) {
    auto a = vector(1, 2, 3);
    auto b = vector(2, 3, 4);
    EXPECT_EQ(dot(a, b), 20);
}

TEST(Tuples, Cross) {
    auto a = vector(1, 2, 3);
    auto b = vector(2, 3, 4);
    EXPECT_EQ(cross(a, b), vector(-1, 2, -1));
    EXPECT_EQ(cross(b, a), vector(1, -2, 1));
}

TEST(Tuples, Color) {
    auto c = color(-0.5, 0.4, 1.7);
    EXPECT_EQ(c.r, -0.5);
    EXPECT_EQ(c.g, 0.4);
    EXPECT_EQ(c.b, 1.7);
}

TEST(Tuples, ColorArithmetic) {
    auto c1 = color(0.9, 0.6, 0.75);
    auto c2 = color(0.7, 0.1, 0.25);
    EXPECT_EQ(c1 + c2, color(1.6, 0.7, 1.0));
    EXPECT_EQ(c1 - c2, color(0.2, 0.5, 0.5));
    
    auto c = color(0.2, 0.3, 0.4);
    EXPECT_EQ(c * 2, color(0.4, 0.6, 0.8));
}

TEST(Tuples, ColorMultiplcation) {
    auto c1 = color(1, 0.2, 0.4);
    auto c2 = color(0.9, 1, 0.1);
    EXPECT_EQ(c1 * c2, color(0.9, 0.2, 0.04));
}