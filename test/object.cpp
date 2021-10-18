#include <gtest/gtest.h>
#include "object.h"
#include "sphere.h"
#include "transform.h"

TEST(Object, DefaultTransform) {
    no_hit_obj_t s;
    EXPECT_EQ(s.transform, identity_matrix);
}

// XXX I'm not adding a saved_ray field to every object just to test some intermediate code
TEST(Object, IntersectScaled) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    no_hit_obj_t s;
    s.transformation(scaling(2, 2, 2));
    auto xs = intersects(s, r);
}

TEST(Object, IntersectTranslated) {
    auto r = ray(point(0, 0, -5), vector(0, 0, 1));
    no_hit_obj_t s;
    s.transformation(translation(5, 0, 0));
    auto xs = intersects(s, r);
}

TEST(Object, Normal) {
    no_hit_obj_t s;
    s.transformation(translation(0, 1, 0));
    auto n = normal_at(s, point(0, 1, 1));
}
