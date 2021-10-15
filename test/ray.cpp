#include <gtest/gtest.h>
#include "ray.h"
#include "transform.h"

TEST(Ray, Create) {
    auto origin = point(1, 2, 3);
    auto direction = vector(4, 5, 6);
    auto r = ray(origin, direction);
    EXPECT_EQ(r.origin, origin);
    EXPECT_EQ(r.direction, direction);
}

TEST(Ray, Position) {
    auto r = ray(point(2, 3, 4), vector(1, 0, 0));
    EXPECT_EQ(position(r, 0), point(2, 3, 4));
    EXPECT_EQ(position(r, 1), point(3, 3, 4));
    EXPECT_EQ(position(r, -1), point(1, 3, 4));
    EXPECT_EQ(position(r, 2.5), point(4.5, 3, 4));
}

TEST(Ray, Translate) {
    auto r = ray(point(1, 2, 3), vector(0, 1, 0));
    auto m = translation(3, 4, 5);
    auto r2 = transform(r, m);
    EXPECT_EQ(r2.origin, point(4, 6, 8));
    EXPECT_EQ(r2.direction, vector(0, 1, 0));
}

TEST(Ray, Scale) {
    auto r = ray(point(1, 2, 3), vector(0, 1, 0));
    auto m = scaling(2, 3, 4);
    auto r2 = transform(r, m);
    EXPECT_EQ(r2.origin, point(2, 6, 12));
    EXPECT_EQ(r2.direction, vector(0, 3, 0));
}