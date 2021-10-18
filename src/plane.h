#ifndef PLANE_H
#define PLANE_H

#include "object.h"

struct plane : public object {
    virtual void local_intersects_in(struct ray const & ray, struct intersections& xs) const {
        if (compare(ray.direction.y, 0.0)) return;

        xs.add(intersection(-ray.origin.y / ray.direction.y, *this));
    }

    virtual struct tuple local_normal_at(struct tuple const& p) const {
        return vector(0, 1, 0);
    }
};

#endif