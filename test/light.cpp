#include <gtest/gtest.h>
#include "light.h"

TEST(Light, Create) {
    auto p = point(0, 0, 0);
    auto c = color(1, 1, 1);
    auto l = point_light(p, c);
    EXPECT_EQ(l.position, p);
    EXPECT_EQ(l.intensity, c);
}