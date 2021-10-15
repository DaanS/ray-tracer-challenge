#include <iostream>
#include <fstream>
#include "tuple.h"
#include "canvas.h"
#include "sphere.h"
#include "transform.h"

struct tuple screen_to_world(struct canvas const& c, double x, double y) {
    return point(x / (c.width / 2.0) - 1, y / (c.height / 2.0) - 1, 1);
}

int main() {
    auto c = canvas(400, 400);
    auto s = sphere();
    s.transform = translation(0, 0, 2);
    auto cam_pos = point(0, 0, 0);

    for (int y = 0; y < c.height; ++y) {
        for (int x = 0; x < c.width; ++x) {
            auto target = screen_to_world(c, x, y);
            auto r = ray(cam_pos, target - cam_pos);
            auto xs = intersects(s, r);
            auto h = hit(xs);
            if (h != no_hit) {
                write_pixel(c, x, y, color(1, 0, 0));
            }
        }
    }

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(c) << std::endl;
}