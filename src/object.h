#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include "matrix.h"
#include "material.h"
#include "intersection.h"

struct object {
    struct matrix_t<4> transform = identity_matrix;
    mutable struct matrix_t<4> inverse_transform = identity_matrix;
    std::unique_ptr<struct material> material_ptr = std::make_unique<struct color_material>();

    virtual ~object() {}

    struct material& material() {
        return *material_ptr;
    }

    struct material const& material() const {
        return *material_ptr;
    }

    template<typename MatType>
    void material(MatType const& mat) {
        material_ptr = std::make_unique<MatType>(mat);
    }

    virtual bool operator==(struct object const& rhs) const { 
        return transform == rhs.transform && material() == rhs.material(); 
    }

    virtual void transformation(struct matrix_t<4> t) {
        assert(is_invertible(t));
        this->transform = t;
        this->inverse_transform = inverse(t);
    }

    virtual void local_intersects_in(struct ray const& r, struct intersections& xs) const = 0;
    virtual void intersects_in(struct ray const& r, struct intersections& xs) const {
        local_intersects_in(::transform(r, this->inverse_transform), xs);
    }

    virtual struct tuple local_normal_at(struct tuple const& p) const = 0;
    virtual struct tuple normal_at(struct tuple const& p) const {
        auto p_obj = this->inverse_transform * p;
        auto n_obj = local_normal_at(this->inverse_transform * p);
        auto n = transpose(inverse(this->transform)) * n_obj;
        n.w = 0;
        return normalize(n);
    }

    friend std::ostream& operator<<(std::ostream& os, struct object const& o) {
        os << "{transform: " << o.transform << ", material: " << o.material() << "}";
        return os;
    }
};

struct no_hit_obj_t : public object {
    virtual void local_intersects_in(struct ray const& r, struct intersections& xs) const {}
    virtual struct tuple local_normal_at(struct tuple const& p) const { return point(0, 0, 0); }
};
static const struct no_hit_obj_t no_hit_obj;

struct tuple normal_at(struct object const& o, struct tuple const& p) {
    return o.normal_at(p);
}

struct intersections intersects(struct object const& o, struct ray const& r) {
    struct intersections xs;
    o.intersects_in(r, xs);
    return xs;
}

struct computations prepare_computations(struct intersection const& i, struct ray const& r) {
    struct computations comps;
    comps.t = i.t;
    comps.obj = &(i.object());
    comps.point = position(r, i.t);
    comps.eye = -r.direction;
    auto normal = normal_at(i.object(), comps.point);
    comps.inside = (dot(normal, comps.eye) < 0);
    comps.normal = normal * (comps.inside ? -1 : 1);
    return comps;
}

#endif