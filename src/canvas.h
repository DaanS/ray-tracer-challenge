#ifndef CANVAS_H
#define CANVAS_H

#include <cassert>
#include <sstream>
#include <vector>
#include "tuple.h"

struct canvas {
    int width;
    int height;
    int depth;
    std::vector<double> pix;
};

static constexpr int depth = 3;

struct canvas canvas(int width, int height) {
    return {width, height, depth, std::vector<double>(width * height * depth, 0)};
}

void assert_pos(struct canvas const& c, int x, int y) {
    assert(x >= 0 && x < c.width);
    assert(y >= 0 && y < c.height);
}

size_t pix_start(struct canvas const& c, int x, int y) {
    return c.depth * (x + y * c.width);
}

struct tuple pixel_at(struct canvas const& c, int x, int y) {
    assert_pos(c, x, y);
    size_t start = pix_start(c, x, y);
    return color(c.pix[start], c.pix[start + 1], c.pix[start + 2]);
}

void write_pixel(struct canvas& c, int x, int y, struct tuple col) {
    assert_pos(c, x, y);
    size_t start = pix_start(c, x, y);
    c.pix[start] = col.r;
    c.pix[start + 1] = col.g;
    c.pix[start + 2] = col.b;
}

void split_ppm_line(std::string& line) {
    size_t start = 0;
    while (line.length() - start > 70) {
        size_t space_pos = line.find_last_of(" ", start + 70);
        assert(space_pos != std::string::npos);
        line[space_pos] = '\n';
        start = space_pos;
    }
}

int col_to_int(double col) {
    if (col < 0) return 0;
    else if (col > 1) return 255;
    else return col * 255;
}

std::string canvas_to_ppm(struct canvas const& c) {
    std::stringstream ss;
    ss << "P3\n" << c.width << " " << c.height << "\n255\n" ;
    for (int y = 0; y < c.height; ++y) {
        std::stringstream line;
        for (int x = 0; x < c.width; ++x) {
            auto col = pixel_at(c, x, y);
            line << col_to_int(col.r) << " " << col_to_int(col.g) << " " << col_to_int(col.b) << " ";
        }
        auto line_str = line.str();
        split_ppm_line(line_str);
        ss << line_str << "\n";
    }
    return ss.str();
}

struct tuple screen_to_world(struct canvas const& c, double x, double y) {
    return point(x / (c.width / 2.0) - 1, 1 - (y / (c.height / 2.0)), 1);
}

#endif