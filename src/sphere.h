#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "intersection.h"
#include "object.h"
#include "ray.h"
#include "tuple.h"

struct sphere : public object { 
    virtual void intersects_in(struct ray const & ray, struct intersections& xs) const {
        //auto r = ::transform(ray, inverse(this->transform));
        auto r = ::transform(ray, this->inverse_transform);
        auto sphere_to_ray = r.origin - point(0, 0, 0);
        auto a = dot(r.direction, r.direction);
        auto b = 2 * dot(r.direction, sphere_to_ray);
        auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

        auto d = b * b - 4 * a *c;

        if (d < 0) return;
        
        xs.count += 2;
        xs.xs.push_back(intersection((-b - sqrt(d)) / (a * 2), *this));
        xs.xs.push_back(intersection((-b + sqrt(d)) / (a * 2), *this));
    }

    virtual struct tuple normal_at(struct tuple const& p) const {
        //auto p_obj = inverse(this->transform) * p;
        auto p_obj = this->inverse_transform * p;
        auto n_obj = p_obj - point(0, 0, 0);
        auto n = transpose(inverse(this->transform)) * n_obj;
        n.w = 0;
        return normalize(n);
    }
};

struct sphere sphere() {
    return {};
}

struct intersections intersects(struct sphere const& s, struct ray const& ray) {
    struct intersections xs;
    s.intersects_in(ray, xs);
    return xs;
}

#endif