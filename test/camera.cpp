#include <gtest/gtest.h>
#include <numbers>
#include "camera.h"
#include "sphere.h"

using namespace std::numbers;

TEST(Camera, Create) {
    auto w = 160;
    auto h = 120;
    auto fov = pi / 2;
    auto c = camera(w, h, fov);
    EXPECT_EQ(c.w, w);
    EXPECT_EQ(c.h, h);
    EXPECT_EQ(c.fov, fov);
    EXPECT_EQ(c.transform, identity_matrix);
}

TEST(Camera, PixSize) {
    auto c = camera(200, 125, pi / 2);
    EXPECT_DOUBLE_EQ(c.pixel_size, 0.01);

    c = camera(125, 200, pi / 2);
    EXPECT_DOUBLE_EQ(c.pixel_size, 0.01);
}

TEST(Camera, RayCenter) {
    auto c = camera(201, 101, pi / 2);
    auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin, point(0, 0, 0));
    EXPECT_EQ(r.direction, vector(0, 0, -1));
}

TEST(Camera, RayCorner) {
    auto c = camera(201, 101, pi / 2);
    auto r = ray_for_pixel(c, 0, 0);
    EXPECT_EQ(r.origin, point(0, 0, 0));
    EXPECT_EQ(r.direction, vector(0.66519, 0.33259, -0.66851));
}

TEST(Camera, RayTransformed) {
    auto c = camera(201, 101, pi / 2);
    c.transformation(rotation_y(pi / 4) * translation(0, -2, 5));
    auto r = ray_for_pixel(c, 100, 50);
    EXPECT_EQ(r.origin, point(0, 2, -5));
    EXPECT_EQ(r.direction, vector(std::sqrt(2) / 2, 0, -std::sqrt(2) / 2));
}

TEST(Camera, Render) {
    struct world w;
    auto l = point_light(point(-10, 10, -10), color(1, 1, 1));
    w.light(l);

    auto& s1 = w.object<struct sphere>();
    s1.material(color_material(0.8, 1.0, 0.6));
    s1.material().diffuse = 0.7;
    s1.material().specular = 0.2;

    auto& s2 = w.object<struct sphere>();
    s2.transform = scaling(0.5, 0.5, 0.5);

    auto c = camera(11, 11, pi / 2);
    auto from = point(0, 0, -5);
    auto to = point(0, 0, 0);
    auto up = vector(0, 1, 0);
    c.transformation(view_transform(from, to, up));
    auto img = render(c, w);
    EXPECT_EQ(pixel_at(img, 5, 5), color(0.38066, 0.47583, 0.2855));
}