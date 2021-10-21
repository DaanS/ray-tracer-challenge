#include <gtest/gtest.h>
#include "intersection.h"
#include "sphere.h"
#include "plane.h"
#include "transform.h"

struct RefractionTest : public ::testing::Test {
    protected:
    struct sphere glass_sphere = sphere();

    void SetUp() {
        glass_sphere.material(color_material(1, 1, 1));
        glass_sphere.material().transparency = 1;
        glass_sphere.material().refractive_index = 1.5;
    }

    struct sphere make_sphere() {
        struct sphere s = sphere();
        s.material(color_material(1, 1, 1));
        s.material().transparency = 1;
        s.material().refractive_index = 1.5;
        return s;
    }
};

TEST(Intersection, Basic) {
    auto s = sphere();
    auto i = intersection(3.5, s);
    EXPECT_EQ(i.t, 3.5);
    EXPECT_EQ(i.object(), s);
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

TEST(Intersection, Precompute) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 01));
    auto shape = sphere();
    auto i = intersection(4, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(comps.t, i.t);
    EXPECT_EQ(comps.object(), i.object());
    EXPECT_EQ(comps.point, point(0, 0, -1));
    EXPECT_EQ(comps.eye, vector(0, 0, -1));
    EXPECT_EQ(comps.normal, vector(0, 0, -1));
    EXPECT_FALSE(comps.inside);
}

TEST(Intersection, Reflect) {
    auto shape = plane();
    auto r = ray(point(0, 1, -1), vector(0, -half_sqrt_2, half_sqrt_2));
    auto i = intersection(std::sqrt(2), shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(comps.reflect, vector(0, half_sqrt_2, half_sqrt_2));
}

TEST(Intersection, Inside) {
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto shape = sphere();
    auto i = intersection(1, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(comps.point, point(0, 0, 1));
    EXPECT_EQ(comps.eye, vector(0, 0, -1));
    EXPECT_EQ(comps.normal, vector(0, 0, -1));
    EXPECT_TRUE(comps.inside);
}

TEST(Intersection, Overpoint) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto shape = sphere();
    shape.transformation(translation(0, 0, 1));
    auto i = intersection(5, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_LT(comps.over_point.z, epsilon / 2);
    EXPECT_GT(comps.point.z, comps.over_point.z);
}

TEST_F(RefractionTest, Underpoint) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    glass_sphere.transformation(translation(0, 0, 1));
    auto i = intersection(5, glass_sphere);
    auto xs = intersections(i);
    auto comps = prepare_computations(i, r);
    EXPECT_GT(comps.under_point.z, epsilon / 2);
    EXPECT_LT(comps.point.z, comps.under_point.z);
}

TEST_F(RefractionTest, GlassSphere) {
    EXPECT_EQ(glass_sphere.transform, identity_matrix);
    EXPECT_EQ(glass_sphere.material().transparency, 1);
    EXPECT_EQ(glass_sphere.material().refractive_index, 1.5);
}

TEST_F(RefractionTest, Intersections) {
    auto a = make_sphere();
    a.transformation(scaling(2, 2, 2));
    a.material().refractive_index = 1.5;
    auto b = make_sphere();
    b.transformation(translation(0, 0, -0.25));
    b.material().refractive_index = 2.0;
    auto c = make_sphere();
    c.transformation(translation(0, 0, 0.25));
    c.material().refractive_index = 2.5;
    auto r = ray(point(0, 0, -4), vector(0, 0, 1));
    auto xs = intersections(
        intersection(2, a), 
        intersection(2.75, b),
        intersection(3.25, c),
        intersection(4.75, b),
        intersection(5.25, c),
        intersection(6, a)
    );
    struct res_t { double n1, n2;};
    auto res = std::vector<res_t>{
        {1.0, 1.5},
        {1.5, 2.0},
        {2.0, 2.5},
        {2.5, 2.5},
        {2.5, 1.5},
        {1.5, 1.0}
    };
    for (int i = 0; i < res.size(); ++i) {
        auto comps = prepare_computations(xs[i], r, xs);
        EXPECT_EQ(comps.n1, res[i].n1);
        EXPECT_EQ(comps.n2, res[i].n2);
    }
}

TEST_F(RefractionTest, SchlickTotal) {
    auto r = ray(point(0, 0, half_sqrt_2), vector(0, 1, 0));
    auto xs = intersections(intersection(-half_sqrt_2, glass_sphere), intersection(half_sqrt_2, glass_sphere));
    auto comps = prepare_computations(xs[1], r, xs);
    EXPECT_EQ(schlick(comps), 1);
}

TEST_F(RefractionTest, SchlickPerpendicular) {
    auto r = ray(point(0, 0, 0), vector(0, 1, 0));
    auto xs = intersections(intersection(-1, glass_sphere), intersection(1, glass_sphere));
    auto comps = prepare_computations(xs[1], r, xs);
    EXPECT_DOUBLE_EQ(schlick(comps), 0.04);
}

TEST_F(RefractionTest, SchlickSmall) {
    auto r = ray(point(0, 0.99, -2), vector(0, 0, 1));
    auto xs = intersections(intersection(1.8589, glass_sphere));
    auto comps = prepare_computations(xs[0], r, xs);
    EXPECT_NEAR(schlick(comps), 0.48873, epsilon);
}