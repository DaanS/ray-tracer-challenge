#include <gtest/gtest.h>
#include "canvas.h"
#include "tuple.h"

std::vector<std::string> split_lines(std::string s) {
    std::stringstream ss(s);
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
}

TEST(Canvas, Create) {
    auto c = canvas(10, 20);
    EXPECT_EQ(c.width, 10);
    EXPECT_EQ(c.height, 20);
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 20; ++y) {
            EXPECT_EQ(pixel_at(c, x, y), color(0, 0, 0));
        }
    }
}

TEST(Canvas, Render) {
    auto c = canvas(10, 20);
    auto red = color(1, 0, 0);
    write_pixel(c, 2, 3, red);
    EXPECT_EQ(pixel_at(c, 2, 3), red); 
}

TEST(Canvas, Header) {
    auto c = canvas(5, 3);
    auto ppm = canvas_to_ppm(c);
    EXPECT_TRUE(ppm.starts_with("P3\n5 3\n255\n"));
}

TEST(Canvas, Pixels) {
    auto c = canvas(5, 3);
    auto c1 = color(1.5, 0, 0);
    auto c2 = color(0, 0.5, 0);
    auto c3 = color(-0.5, 0, 1);
    write_pixel(c, 0, 0, c1);
    write_pixel(c, 2, 1, c2);
    write_pixel(c, 4, 2, c3);
    auto ppm = canvas_to_ppm(c);
    auto lines = split_lines(ppm);
    EXPECT_EQ(lines[3], "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
    EXPECT_EQ(lines[4], "0 0 0 0 0 0 0 127 0 0 0 0 0 0 0 ");
    EXPECT_EQ(lines[5], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 ");
}