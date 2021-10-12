#include <iostream>
#include <fstream>
#include "tuple.h"
#include "canvas.h"

struct projectile {
    struct tuple pos;
    struct tuple vel;
};

struct env {
    struct tuple g;
    struct tuple w;
};

int main() {
    auto p = projectile{point(0, 1, 0), normalize(vector(1, 1.8, 0)) * 11.25};
    auto e = env{vector(0, -0.1, 0), vector(-0.01, 0, 0)};
    auto c = canvas(1200, 550);

    while (p.pos.y > -p.vel.y) {
        p.pos += p.vel;
        p.vel += e.g;
        p.vel += e.w;

        std::cout << "tick: " << p.pos << std::endl;
        write_pixel(c, p.pos.x, 550 - p.pos.y, color(1, 0, 0));
    }

    std::ofstream ofs("out.ppm");
    ofs << canvas_to_ppm(c) << std::endl;
}