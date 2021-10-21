#include <numbers>
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
    EXPECT_EQ(xs[0].object(), s);
    EXPECT_EQ(xs[1].object(), s);
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
    s.transformation(t);
    EXPECT_EQ(s.transform, t);
}

TEST(Sphere, IntersectScaled) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    s.transformation(scaling(2, 2, 2));
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 2);
    EXPECT_EQ(xs[0].t, 3);
    EXPECT_EQ(xs[1].t, 7);
}

TEST(Sphere, IntersectTranslated) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto s = sphere();
    s.transformation(translation(5, 0, 0));
    auto xs = intersects(s, r);
    EXPECT_EQ(xs.count, 0);
}

TEST(Sphere, Normal) {
    auto s = sphere();
    EXPECT_EQ(normal_at(s, point(1, 0, 0)), vector(1, 0, 0));
    EXPECT_EQ(normal_at(s, point(0, 1, 0)), vector(0, 1, 0));
    EXPECT_EQ(normal_at(s, point(0, 0, 1)), vector(0, 0, 1));

    auto three = std::sqrt(3) / 3;
    auto n = normal_at(s, point(three, three, three));
    EXPECT_EQ(n, vector(three, three, three));
    EXPECT_EQ(n, normalize(n));
}

TEST(Sphere, NormalTranslated) {
    auto s = sphere();
    s.transformation(translation(0, 1, 0));
    auto n = normal_at(s, point(0, 1.70711, -0.70711));
    EXPECT_EQ(n, vector(0, 0.70711, -0.70711));
}

TEST(Sphere, NormalTransformed) {
    auto s = sphere();
    s.transformation(scaling(1, 0.5, 1) * rotation_z(std::numbers::pi / 5));
    auto n = normal_at(s, point(0, half_sqrt_2, -half_sqrt_2));
    EXPECT_EQ(n, vector(0, 0.97014, -0.24254));
}

TEST(Sphere, Material) {
    auto s = sphere();
    auto m = color_material();
    EXPECT_EQ(s.material(), m);

    m.ambient = 1;
    s.material(m);
    EXPECT_EQ(s.material(), m);
    EXPECT_EQ(s.material().ambient, 1);
}