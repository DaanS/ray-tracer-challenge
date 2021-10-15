#include <gtest/gtest.h>
#include "intersection.h"
#include "sphere.h"

TEST(Intersection, Basic) {
    auto s = sphere();
    auto i = intersection(3.5, s);
    EXPECT_EQ(i.t, 3.5);
    EXPECT_EQ(i.object, s);
}

TEST(Intersection, Aggregate) {
    auto s = sphere();
    auto i1 = intersection(1, s);
    auto i2 = intersection(2, s);
    auto xs = intersections(i1, i2);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, 1);
    EXPECT_EQ(xs[1].t, 2);
}

TEST(Intersection, HitPositive) {
    auto s = sphere();
    auto i1 = intersection(1, s);
    auto i2 = intersection(2, s);
    auto xs = intersections(i2, i1);
    EXPECT_EQ(hit(xs), i1);
}

TEST(Intersection, HitMixed) {
    auto s = sphere();
    auto i1 = intersection(-1, s);
    auto i2 = intersection(1, s);
    auto xs = intersections(i2, i1);
    EXPECT_EQ(hit(xs), i2);
}

TEST(Intersection, HitNegative) {
    auto s = sphere();
    auto i1 = intersection(-2, s);
    auto i2 = intersection(-1, s);
    auto xs = intersections(i2, i1);
    EXPECT_EQ(hit(xs), no_hit);
}

TEST(Intersection, Order) {
    auto s = sphere();
    auto i1 = intersection(5, s);
    auto i2 = intersection(7, s);
    auto i3 = intersection(-3, s);
    auto i4 = intersection(2, s);
    auto xs = intersections(i1, i2, i3, i4);
    EXPECT_EQ(hit(xs), i4);
}