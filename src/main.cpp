#include <iostream>
#include <fstream>
#include <numbers>
#include "tuple.h"
#include "canvas.h"
#include "sphere.h"
#include "transform.h"
#include "world.h"
#include "camera.h"
#include "plane.h"
#include "util.h"

using namespace std::numbers;

struct world build_world() {
    struct world w;

    auto& floor = w.object<struct plane>();
    auto wall_mat = pattern<stripe>(color(1, 0.5, 1), color(1, 1, 1));
    wall_mat.specular = 0;
    floor.transformation(rotation_y(-0.3) * translation(-0.5, 0, 0));
    floor.material(color_material(1, 1, 1));
    floor.material().reflective = 0.5;

    auto& left_wall = w.object<struct plane>();
    left_wall.transformation(translation(0, 0, 5) * rotation_y(-0.3) * rotation_x(pi / 2));
    left_wall.material(wall_mat);
    left_wall.material().reflective = 0.5;

    //auto& right_wall = w.object<struct plane>();
    //right_wall.transformation(translation(0, 0, -15) * rotation_y(-0.3) * rotation_x(pi / 2));
    //right_wall.material(wall_mat);

    auto& middle = w.object<struct sphere>();
    middle.transform = translation(-0.5, 1, 0.5);
    middle.material(color_material(0.1, 1, 0.5));
    middle.material().reflective = 0.5;
    middle.material().diffuse = 0.7;
    middle.material().specular = 0.3;

    auto& right = w.object<struct sphere>();
    right.transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right.material(color_material(0.5, 1, 0.5));
    right.material().reflective = 0.2;
    right.material().diffuse = 0.7;
    right.material().specular = 0.3;

    auto& left = w.object<struct sphere>();
    left.transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
    left.material(color_material(1, 0.5, 1));
    left.material().reflective = 0.2;
    left.material().diffuse = 0.7;
    left.material().specular = 0.3;

    auto light = point_light(point(-10, 10, -10), color(1, 1, 0.5));
    w.light(light);

    auto light2 = point_light(point(10, 10, -10), color(0, 0.5, 1));
    w.light(light2);

    for (auto const& obj : w.objects_container) obj->inverse_transform = inverse(obj->transform);

    return w;
}

int main() {
    auto w = build_world();
    auto cam = camera(1600, 900, pi / 3);
    cam.transform = view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));
    auto canvas = render(cam, w);

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(canvas) << std::endl;
}