#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "ray.h"
#include "world.h"
#include "canvas.h"

struct camera {
    size_t w;
    size_t h;
    double fov;
    struct matrix_t<4> transform = identity_matrix;;
    mutable struct matrix_t<4> inverse_transform = identity_matrix;;
    double pixel_size;

    // in worldspace
    double half_w;
    double half_h;

    void transformation(struct matrix_t<4> t) {
        this->transform = t;
        this->inverse_transform = inverse(t);
    }
};

struct camera camera(size_t w, size_t h, double fov) {
    struct camera cam;
    cam.w = w;
    cam.h = h;
    cam.fov = fov;

    auto half_view = tan(cam.fov / 2);
    auto aspect = double(w) / h;
    if (aspect >= 1) {
        cam.half_w = half_view;
        cam.half_h = half_view / aspect;
    } else {
        cam.half_w = half_view * aspect;
        cam.half_h = half_view;
    }
    cam.pixel_size = (cam.half_w * 2) / cam.w;
    return cam;
}

struct ray ray_for_pixel(struct camera const& cam, size_t x, size_t y) {
    auto offset_x = (x + 0.5) * cam.pixel_size;
    auto offset_y = (y + 0.5) * cam.pixel_size;
    auto world_x = cam.half_w - offset_x;
    auto world_y = cam.half_h - offset_y;

    auto inv = cam.inverse_transform;
    auto pixel = inv * point(world_x, world_y, -1);
    auto origin = inv * point(0, 0, 0);
    return {origin, normalize(pixel - origin)};
}

void print_progress(double prog) {
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * prog;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(prog * 100.0) << " %\r";
    std::cout.flush();
}

struct canvas render(struct camera const& cam, struct world const& w) {
    struct canvas img = canvas(cam.w, cam.h);
    cam.inverse_transform = inverse(cam.transform);
    for (int y = 0; y < cam.h; ++y) {
        for (int x = 0; x < cam.w; ++x) {
            write_pixel(img, x, y, color_at(w, ray_for_pixel(cam, x, y)));
        }
        print_progress(double(y) / cam.h);
    }
    std::cout << std::endl;
    return img;
}

#endif