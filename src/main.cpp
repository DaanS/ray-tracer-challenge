#include <iostream>
#include <fstream>
#include "tuple.h"
#include "canvas.h"
#include "sphere.h"
#include "transform.h"

int main() {
    auto c = canvas(400, 400);

    auto s = sphere();
    s.transform = translation(0, 0, 2);
    s.material.color = color(1, 0.2, 1);

    auto light = point_light(point(-10, 10, -10), color(1, 1, 1));

    auto cam_pos = point(0, 0, 0);

    for (int y = 0; y < c.height; ++y) {
        for (int x = 0; x < c.width; ++x) {
            auto target = screen_to_world(c, x, y);
            auto r = ray(cam_pos, normalize(target - cam_pos));
            auto xs = intersects(s, r);
            auto h = hit(xs);
            if (h != no_hit) {
                auto point = position(r, h.t);
                auto normal = normal_at(dynamic_cast<sphere const&>(h.object), point);
                auto eye = -r.direction;
                auto color = lighting(h.object.material, light, point, eye, normal);
                write_pixel(c, x, y, color);
            }
        }
    }

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(c) << std::endl;
}