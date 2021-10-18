#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "intersection.h"
#include "object.h"
#include "ray.h"
#include "tuple.h"

struct sphere : public object { 
    virtual void local_intersects_in(struct ray const & ray, struct intersections& xs) const {
        auto sphere_to_ray = ray.origin - point(0, 0, 0);
        auto a = dot(ray.direction, ray.direction);
        auto b = 2 * dot(ray.direction, sphere_to_ray);
        auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

        auto d = b * b - 4 * a *c;

        if (d < 0) return;
        
        xs.add(intersection((-b - sqrt(d)) / (a * 2), *this));
        xs.add(intersection((-b + sqrt(d)) / (a * 2), *this));
    }

    virtual struct tuple local_normal_at(struct tuple const& p) const {
        return p - point(0, 0, 0);
    }
};

struct sphere sphere() {
    return {};
}

#endif