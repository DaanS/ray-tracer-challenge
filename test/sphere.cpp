#include <gtest/gtest.h>
#include "sphere.h"
#include "transform.h"

TEST(Sphere, Intersect) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, 4);
    EXPECT_EQ(xs[1].t, 6);
    EXPECT_EQ(xs[0].object, s);
    EXPECT_EQ(xs[1].object, s);
}

TEST(Sphere, Tangent) {
    auto r = ray(point(0, 1, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, 5);
    EXPECT_EQ(xs[1].t, 5);
}

TEST(Sphere, Miss) {
    auto r = ray(point(0, 2, -5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 0);
}

TEST(Sphere, Inside) {
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, -1);
    EXPECT_EQ(xs[1].t, 1);
}

TEST(Sphere, Behind) {
    auto r = ray(point(0, 0, 5), vector(0, 0, 1));
    auto s = sphere();
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, -6);
    EXPECT_EQ(xs[1].t, -4);
}

TEST(Sphere, Create) {
    auto s = sphere();
    EXPECT_EQ(s.transform, identity_matrix);
}

TEST(Sphere, Transform) {
    auto s = sphere();
    auto t = translation(2, 3, 4);
    s.transform = t;
    EXPECT_EQ(s.transform, t);
}

TEST(Sphere, IntersectScaled) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    s.transform = scaling(2, 2, 2);
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, 3);
    EXPECT_EQ(xs[1].t, 7);
}

TEST(Sphere, IntersectTranslated) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    s.transform = translation(5, 0, 0);
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 0);
}