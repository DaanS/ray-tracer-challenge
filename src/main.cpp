#include <iostream>
#include <fstream>
#include <numbers>
#include "tuple.h"
#include "canvas.h"
#include "sphere.h"
#include "transform.h"
#include "world.h"
#include "camera.h"

using namespace std::numbers;

void draw_purple_sphere_in_void() {
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
                auto normal = h.object().normal_at(point);
                auto eye = -r.direction;
                auto color = lighting(h.object().material, light, point, eye, normal);
                write_pixel(c, x, y, color);
            }
        }
    }

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(c) << std::endl;
}

struct world build_world() {
    struct world w;

    auto floor = sphere();
    floor.transform = scaling(10, 0.01, 10);
    floor.material = material();
    floor.material.color = color(1, 0.9, 0.9);
    floor.material.specular = 0;
    w.object(floor);

    auto left_wall = sphere();
    left_wall.transform = translation(0, 0, 5) * rotation_y(-pi / 4) * rotation_x(pi / 2) * scaling(10, 0.01, 10);
    left_wall.material = floor.material;
    w.object(left_wall);

    auto right_wall = sphere();
    right_wall.transform = translation(0, 0, 5) * rotation_y(pi / 4) * rotation_x(pi / 2) * scaling(10, 0.01, 10);
    right_wall.material = floor.material;
    w.object(right_wall);

    auto middle = sphere();
    middle.transform = translation(-0.5, 1, 0.5);
    middle.material = material();
    middle.material.color = color(0.1, 1, 0.5);
    middle.material.diffuse = 0.7;
    middle.material.specular = 0.3;
    w.object(middle);

    auto right = sphere();
    right.transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right.material = material();
    right.material.color = color(0.5, 1, 0.5);
    right.material.diffuse = 0.7;
    right.material.specular = 0.3;
    w.object(right);

    auto left = sphere();
    left.transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
    left.material = material();
    left.material.color = color(1, 0.5, 1);
    left.material.diffuse = 0.7;
    left.material.specular = 0.3;
    w.object(left);

    auto light = point_light(point(-10, 10, -10), color(1, 1, 0.5));
    w.light(light);

    auto light2 = point_light(point(10, 10, -10), color(0, 0.5, 1));
    w.light(light2);

    for (auto const& obj : w.objects_container) obj->inverse_transform = inverse(obj->transform);

    return w;
}

int main() {
    auto w = build_world();
    auto cam = camera(800, 600, pi / 3);
    cam.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));
    auto canvas = render(cam, w);

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(canvas) << std::endl;
}