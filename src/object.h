#ifndef OBJECT_H
#define OBJECT_H

#include "matrix.h"
#include "material.h"

struct object {
    struct matrix_t<4> transform = identity_matrix;
    mutable struct matrix_t<4> inverse_transform = identity_matrix;
    struct material material;

    virtual ~object() {}

    virtual bool operator==(struct object const& rhs) const { 
        return transform == rhs.transform && material == rhs.material; 
    }

    virtual void transformation(struct matrix_t<4> t) {
        this->transform = t;
        this->inverse_transform = inverse(t);
    }

    virtual void intersects_in(struct ray const& r, struct intersections& xs) const = 0;

    virtual struct tuple normal_at(struct tuple const& p) const = 0;

    friend std::ostream& operator<<(std::ostream& os, struct object const& o) {
        os << "{transform: " << o.transform << ", material: " << o.material << "}";
        return os;
    }
};

struct no_hit_obj_t : public object {
    virtual void intersects_in(struct ray const& r, struct intersections& xs) const {}
    virtual struct tuple normal_at(struct tuple const& p) const { return point(0, 0, 0); }
};
static const struct no_hit_obj_t no_hit_obj;

struct tuple normal_at(struct object const& o, struct tuple const& p) {
    return o.normal_at(p);
}

#endif