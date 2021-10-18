#include <gtest/gtest.h>
#include "plane.h"

TEST(Plane, Normal) {
    auto p = plane();
    auto n1 = normal_at(p, point(0, 0, 0));
    auto n2 = normal_at(p, point(10, 0, -10));
    auto n3 = normal_at(p, point(-5, 0, 150));
    EXPECT_EQ(n1, vector(0, 1, 0));
    EXPECT_EQ(n2, n1);
    EXPECT_EQ(n3, n1);
}

TEST(Plane, IntersectParallel) {
    auto p = plane();
    auto r = ray(point(0, 10, 0), vector(0, 0, 1));
    auto xs = intersects(p, r);
    EXPECT_EQ(xs.count, 0);
}

TEST(Plane, IntersectAbove) {
    auto p = plane();
    auto r = ray(point(0, 1, 0), vector(0, -1, 0));
    auto xs = intersects(p, r);
    EXPECT_EQ(xs.count, 1);
    EXPECT_EQ(xs[0].t, 1);
    EXPECT_EQ(xs[0].object(), p);
}

TEST(Plane, IntersectBelow) {
    auto p = plane();
    auto r = ray(point(0, -1, 0), vector(0, 1, 0));
    auto xs = intersects(p, r);
    EXPECT_EQ(xs.count, 1);
    EXPECT_EQ(xs[0].t, 1);
    EXPECT_EQ(xs[0].object(), p);
}