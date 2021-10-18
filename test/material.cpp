#include <gtest/gtest.h>
#include "material.h"

struct ColorMaterialTest : public ::testing::Test {
protected:
    struct color_material m;
    struct tuple pos;

    void SetUp() override {
        pos = point(0, 0, 0);
    }
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
    auto res = lighting(m, light, pos, eye, normal);
    EXPECT_EQ(res, color(1.9, 1.9, 1.9));
}

TEST_F(ColorMaterialTest, Eye45) {
    auto eye = vector(0, std::sqrt(2) / 2, -std::sqrt(2) / 2);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, -10), color(1, 1, 1));
    auto res = lighting(m, light, pos, eye, normal);
    EXPECT_EQ(res, color(1, 1, 1));
}

TEST_F(ColorMaterialTest, Light45) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 10, -10), color(1, 1, 1));
    auto res = lighting(m, light, pos, eye, normal);
    EXPECT_EQ(res, color(0.7364, 0.7364, 0.7364));
}

TEST_F(ColorMaterialTest, EyeLight45) {
    auto eye = vector(0, -std::sqrt(2) / 2, -std::sqrt(2) / 2);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 10, -10), color(1, 1, 1));
    auto res = lighting(m, light, pos, eye, normal);
    EXPECT_EQ(res, color(1.6364, 1.6364, 1.6364));
}

TEST_F(ColorMaterialTest, Behind) {
    auto eye = vector(0, 0, -1);
    auto normal = vector(0, 0, -1);
    auto light = point_light(point(0, 0, 10), color(1, 1, 1));
    auto res = lighting(m, light, pos, eye, normal);
    EXPECT_EQ(res, color(0.1, 0.1, 0.1));
}