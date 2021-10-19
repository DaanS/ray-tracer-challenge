#include <gtest/gtest.h>
#include "world.h"
#include "sphere.h"
#include "transform.h"
#include "plane.h"

struct WorldTest : public ::testing::Test {
    struct world w;

    virtual void SetUp() {
        auto l = point_light(point(-10, 10, -10), color(1, 1, 1));
        w.light(l);

        auto& s1 = w.object<struct sphere>();
        s1.material(color_material(0.8, 1.0, 0.6));
        s1.material().diffuse = 0.7;
        s1.material().specular = 0.2;

        auto& s2 = w.object<struct sphere>();
        s2.transformation(scaling(0.5, 0.5, 0.5));
    }
};

TEST(World, Create) {
    auto w = world();
    EXPECT_TRUE(w.lights_container.empty());
    EXPECT_TRUE(w.objects_container.empty());
}

TEST_F(WorldTest, Object) {
    auto& obj = *w.objects_container[0];
    EXPECT_NO_THROW(auto& s = dynamic_cast<struct sphere&>(obj));

    auto& obj3 = w.objects(0);
    EXPECT_NO_THROW(auto& s3 = dynamic_cast<struct sphere&>(obj3));
}

TEST_F(WorldTest, Default) {
    auto light = point_light(point(-10, 10, -10), color(1, 1, 1));
    auto s1 = sphere();
    s1.material(color_material(0.8, 1.0, 0.6));
    s1.material().diffuse = 0.7;
    s1.material().specular = 0.2;
    auto s2 = sphere();
    s2.transformation(scaling(0.5, 0.5, 0.5));

    EXPECT_EQ(w.lights(0), light);
    EXPECT_EQ(w.objects(0), s1);
    EXPECT_EQ(w.objects(1), s2);
}

TEST_F(WorldTest, Intersect) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto xs = intersects(w, r);
    EXPECT_EQ(xs.count, 4);
    EXPECT_EQ(xs[0].t, 4);
    EXPECT_EQ(xs[1].t, 4.5);
    EXPECT_EQ(xs[2].t, 5.5);
    EXPECT_EQ(xs[3].t, 6);
}

TEST_F(WorldTest, Shade) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto& shape = w.objects(0);
    auto i = intersection(4, shape);
    auto comps = prepare_computations(i, r);
    auto c = shade_hit(w, comps);
    EXPECT_EQ(c, color(0.38066, 0.47583, 0.2855));
}

TEST_F(WorldTest, ShadeInside) {
    w.lights(0) = point_light(point(0, 0.25, 0), color(1, 1, 1));
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto& shape = w.objects(1);
    auto i = intersection(0.5, shape);
    auto comps = prepare_computations(i, r);
    auto c = shade_hit(w, comps);
    EXPECT_EQ(c, color(0.90498, 0.90498, 0.90498));
}

TEST_F(WorldTest, MissColor) {
    auto r = ray(point(0, 0, -5), vector(0, 1, 0));
    auto c = color_at(w, r);
    EXPECT_EQ(c, color(0, 0, 0));
}

TEST_F(WorldTest, HitColor) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    auto c = color_at(w, r);
    EXPECT_EQ(c, color(0.38066, 0.47583, 0.2855));
}

TEST_F(WorldTest, MidColor) {
    auto& outer = w.objects(0);
    outer.material().ambient = 1;
    auto& inner = w.objects(1);
    inner.material().ambient = 1;
    auto r = ray(point(0, 0, 0.75), vector(0, 0, -1));
    auto c = color_at(w, r);
    EXPECT_EQ(c, dynamic_cast<color_material&>(inner.material()).color);
}

bool is_shadowed(struct world const& w, struct tuple const& p) {
    return is_shadowed(w, w.lights(0), p);
}

TEST_F(WorldTest, ShadowColinear) {
    auto p = point(0, 10, 0);
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST_F(WorldTest, ShadowBlocked) {
    auto p = point(10, -10, 10);
    EXPECT_TRUE(is_shadowed(w, p));
}

TEST_F(WorldTest, ShadowBehind) {
    auto p = point(-20, 20, -20);
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST_F(WorldTest, ShadowBetween) {
    auto p = point(-2, 2, -2);
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST_F(WorldTest, NonreflectiveColor) {
    auto r = ray(point(0, 0, 0), vector(0, 0, 1));
    auto& shape = w.objects(1);
    shape.material().ambient = 1;
    auto i = intersection(1, shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(reflected_color(w, comps), color(0, 0, 0));
}

TEST_F(WorldTest, ReflectiveColor) {
    auto& shape = w.object<struct plane>();
    shape.material().reflective = 0.5;
    shape.transformation(translation(0, -1, 0));
    auto r = ray(point(0, 0, -3), vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    auto i = intersection(std::sqrt(2), shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(reflected_color(w, comps), color(0.19032, 0.2379, 0.14274));
}

TEST_F(WorldTest, ReflectiveShade) {
    auto& shape = w.object<struct plane>();
    shape.material().reflective = 0.5;
    shape.transformation(translation(0, -1, 0));
    auto r = ray(point(0, 0, -3), vector(0, -std::sqrt(2) / 2, std::sqrt(2) / 2));
    auto i = intersection(std::sqrt(2), shape);
    auto comps = prepare_computations(i, r);
    EXPECT_EQ(shade_hit(w, comps), color(0.87677, 0.92436, 0.82918));
}

TEST_F(WorldTest, Recursion) {
    auto w = world();
    w.light(point_light(point(0, 0, 0), color(1, 1, 1)));
    auto& lower = w.object<plane>();
    lower.material().reflective = 1;
    lower.transformation(translation(0, -1, 0));
    auto& upper = w.object<plane>();
    upper.material().reflective = 1;
    upper.transformation(translation(0, 1, 0));
    auto r = ray(point(0, 0, 0), vector(0, 1, 0));
    color_at(w, r);
}