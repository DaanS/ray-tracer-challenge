#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "intersection.h"
#include "object.h"
#include "ray.h"

struct sphere : public object { 
    sphere() : object(identity_matrix) {}
};

struct intersections intersects(struct sphere const& s, struct ray const& ray) {
    auto r = transform(ray, inverse(s.transform));
    auto sphere_to_ray = r.origin - point(0, 0, 0);
    auto a = dot(r.direction, r.direction);
    auto b = 2 * dot(r.direction, sphere_to_ray);
    auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

    auto d = b * b - 4 * a *c;

    if (d < 0) return {};

    return intersections(
        intersection((-b - sqrt(d)) / (a * 2), s),
        intersection((-b + sqrt(d)) / (a * 2), s)
    );
}

#endif