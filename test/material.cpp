#include <gtest/gtest.h>
#include "material.h"
#include "sphere.h"
#include "transform.h"

struct ColorMaterialTest : public ::testing::Test {
protected:
    struct color_material m;
    struct tuple pos;
    struct sphere s;

    void SetUp() override {
        pos = point(0, 0, 0);
        s.material(m);
    }
};

struct PatternMaterialTest : public ::testing::Test {
    protected:
    struct tuple white = color(1, 1, 1);
    struct tuple black = color(0, 0, 0);
    struct pattern_material<struct stripe> stripe_mat = pattern<stripe>(white, black);

    void SetUp() override { }
};

TEST_F(ColorMaterialTest, Create) {
    EXPECT_EQ(m.color, color(1, 1, 1));
    EXPECT_EQ(m.ambient, 0.1);
    EXPECT_EQ(m.diffuse, 0.9);
    EXPECT_EQ(m.specular, 0.9);
    EXPECT_EQ(m.shininess, 200);
}

TEST_F(ColorMaterialTest, Straight) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, -10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal);
    EXPECT_EQ(res, color(1.9, 1.9, 1.9));
}

TEST_F(ColorMaterialTest, Eye45) {
    auto eye = vector(0, half_sqrt_2, -half_sqrt_2);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, -10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal);
    EXPECT_EQ(res, color(1, 1, 1));
}

TEST_F(ColorMaterialTest, Light45) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 10, -10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal);
    EXPECT_EQ(res, color(0.7364, 0.7364, 0.7364));
}

TEST_F(ColorMaterialTest, EyeLight45) {
    auto eye = vector(0, -half_sqrt_2, -half_sqrt_2);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 10, -10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal);
    EXPECT_EQ(res, color(1.6364, 1.6364, 1.6364));
}

TEST_F(ColorMaterialTest, Behind) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, 10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal);
    EXPECT_EQ(res, color(0.1, 0.1, 0.1));
}

TEST_F(ColorMaterialTest, Shadow) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, -10), color(1, 1, 1));
    auto res = lighting(s, light, pos, eye, normal, true);
    EXPECT_EQ(res, color(0.1, 0.1, 0.1));
}

TEST_F(PatternMaterialTest, Create) {
    EXPECT_EQ(stripe_mat.pattern.c1, white);
    EXPECT_EQ(stripe_mat.pattern.c2, black);
}

TEST_F(PatternMaterialTest, Values) {
    EXPECT_EQ(stripe_mat.color_at(point(0, 0, 0)), white);
    EXPECT_EQ(stripe_mat.color_at(point(0, 1, 0)), white);
    EXPECT_EQ(stripe_mat.color_at(point(0, 2, 0)), white);
    EXPECT_EQ(stripe_mat.color_at(point(0, 0, 1)), white);
    EXPECT_EQ(stripe_mat.color_at(point(0, 0, 2)), white);
    EXPECT_EQ(stripe_mat.color_at(point(0.9, 0, 0)), white);
    EXPECT_EQ(stripe_mat.color_at(point(1, 0, 0)), black);
    EXPECT_EQ(stripe_mat.color_at(point(-0.1, 0, 0)), black);
    EXPECT_EQ(stripe_mat.color_at(point(-1, 0, 0)), black);
    EXPECT_EQ(stripe_mat.color_at(point(-1.1, 0, 0)), white);
}

TEST_F(PatternMaterialTest, Lighting) {
    stripe_mat.ambient = 1;
    stripe_mat.diffuse = 0;
    stripe_mat.specular = 0;
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, -10), color(1, 1, 1));
    auto obj = sphere();
    obj.material(stripe_mat);
    EXPECT_EQ(lighting(obj, light, point(0.9, 0, 0), eye, normal), white);
    EXPECT_EQ(lighting(obj, light, point(1.1, 0, 0), eye, normal), black);
}

TEST_F(PatternMaterialTest, StripeObjectTransform) {
    auto obj = sphere();
    obj.transformation(scaling(2, 2, 2));
    obj.material(stripe_mat);
    EXPECT_EQ(obj.color_at(point(1.5, 0, 0)), white);
}

TEST_F(PatternMaterialTest, StripePatternTransform) {
    auto obj = sphere();
    stripe_mat.transformation(scaling(2, 2, 2));
    obj.material(stripe_mat);
    EXPECT_EQ(obj.color_at(point(1.5, 0, 0)), white);
}

TEST_F(PatternMaterialTest, Gradient) {
    auto pat = pattern<gradient>(white, black);
    EXPECT_EQ(pat.color_at(point(0, 0, 0)), white);
    EXPECT_EQ(pat.color_at(point(0.25, 0, 0)), color(0.75, 0.75, 0.75));
    EXPECT_EQ(pat.color_at(point(0.5, 0, 0)), color(0.5, 0.5, 0.5));
    EXPECT_EQ(pat.color_at(point(0.75, 0, 0)), color(0.25, 0.25, 0.25));
}

TEST_F(PatternMaterialTest, Ring) {
    auto pat = pattern<ring>(white, black);
    EXPECT_EQ(pat.color_at(point(0, 0, 0)), white);
    EXPECT_EQ(pat.color_at(point(1, 0, 0)), black);
    EXPECT_EQ(pat.color_at(point(0, 0, 1)), black);
    EXPECT_EQ(pat.color_at(point(0.708, 0, 0.708)), black);
}

TEST_F(PatternMaterialTest, Checkers) {
    auto pat = pattern<checkers>(white, black);
    EXPECT_EQ(pat.color_at(point(0, 0, 0)), white);
    EXPECT_EQ(pat.color_at(point(0.99, 0, 0)), white);
    EXPECT_EQ(pat.color_at(point(1.01, 0, 0)), black);
    EXPECT_EQ(pat.color_at(point(0, 0.99, 0)), white);
    EXPECT_EQ(pat.color_at(point(0, 1.01, 0)), black);
    EXPECT_EQ(pat.color_at(point(0, 0, 0.99)), white);
    EXPECT_EQ(pat.color_at(point(0, 0, 1.01)), black);
}

TEST_F(ColorMaterialTest, Reflective) {
    EXPECT_EQ(m.reflective, 0);
}

TEST_F(ColorMaterialTest, Refraction) {
    EXPECT_EQ(m.transparency, 0);
    EXPECT_EQ(m.refractive_index, 1);
}