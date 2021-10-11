#include <iostream>
#include "tuple.h"

struct projectile {
    struct tuple pos;
    struct tuple vel;
};

struct env {
    struct tuple g;
    struct tuple w;
};

int main() {
    auto p = projectile{point(0, 1, 0), normalize(vector(1, 1, 0))};
    auto e = env{vector(0, -0.1, 0), vector(-0.01, 0, 0)};

    while (p.pos.y > 0) {
        p.pos += p.vel;
        p.vel += e.g;
        p.vel += e.w;

        std::cout << "tick: " << p.pos << std::endl;
    }
}