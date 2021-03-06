#ifndef OBJECT_H
#define OBJECT_H

#include <algorithm>
#include <memory>
#include "matrix.h"
#include "material.h"
#include "intersection.h"

struct object {
    struct matrix_t<4> transform = identity_matrix;
    mutable struct matrix_t<4> inverse_transform = identity_matrix;
    std::unique_ptr<struct material> material_ptr = std::make_unique<struct color_material>();

    virtual ~object() {}

    object(){}
    object(struct object const&) = delete;
    object(struct object&& other) {
        transform = other.transform;
        inverse_transform = other.inverse_transform;
        material_ptr = std::move(other.material_ptr);
    }
    object& operator=(struct object const&) = delete;
    object& operator=(struct object&& other) {
        std::swap(transform, other.transform);
        std::swap(inverse_transform, other.inverse_transform);
        std::swap(material_ptr, other.material_ptr);
        return *this;
    }

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
        auto n = transpose(this->inverse_transform) * n_obj;
        n.w = 0;
        return normalize(n);
    }

    struct tuple color_at(struct tuple const& p) const {
        return material().color_at(inverse_transform * p);
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

void add_refractives(struct computations& comps, struct intersections const& xs, double t) {
    std::vector<struct object const*> containers;
    for (int i = 0; i < xs.count; ++i) {
        if (xs[i].t == t) {
            if (containers.empty()) comps.n1 = 1;
            else comps.n1 = containers.back()->material().refractive_index;
        }

        auto remove_end = std::remove_if(containers.begin(), containers.end(), [&](struct object const* o){ return *o == xs[i].object(); });
        if (remove_end == containers.end()) containers.push_back(&(xs[i].object()));
        else containers.erase(remove_end, containers.end());

        if (xs[i].t == t) {
            if (containers.empty()) comps.n2 = 1;
            else comps.n2 = containers.back()->material().refractive_index;
        }
    }
}

struct computations prepare_computations(struct intersection const& i, struct ray const& r, struct intersections const& xs) {
    struct computations comps;
    comps.t = i.t;
    comps.obj = &(i.object());
    comps.point = position(r, i.t);
    comps.eye = -r.direction;
    auto normal = normal_at(i.object(), comps.point);
    comps.inside = (dot(normal, comps.eye) < 0);
    comps.normal = normal * (comps.inside ? -1 : 1);
    comps.reflect = reflect(r.direction, comps.normal);
    comps.over_point = comps.point + comps.normal * epsilon;
    comps.under_point = comps.point - comps.normal * epsilon;
    add_refractives(comps, xs, i.t);
    return comps;
}

struct computations prepare_computations(struct intersection const& i, struct ray const& r) {
    return prepare_computations(i, r, intersections(i));
}

struct tuple lighting(struct object const& obj, struct light const& light, struct tuple point, struct tuple eye, struct tuple normal, bool shadow = false) {
    auto base_color = obj.color_at(point) * light.intensity;
    auto ambient = base_color * obj.material().ambient;
    //auto reflect_factor = std::max(0.0, 1 - obj.material().reflective - obj.material().transparency);
    //auto reflect_factor = 1 - obj.material().reflective;
    auto reflect_factor = 1;
    if (shadow) return ambient * reflect_factor;

    auto diffuse = color(0, 0, 0);
    auto specular = color(0, 0, 0);

    auto light_v = normalize(light.position - point);
    auto l_dot_n = dot(light_v, normal);

    if (l_dot_n >= 0) {
        diffuse = base_color * obj.material().diffuse * l_dot_n;

        auto reflect_v = reflect(-light_v, normal);
        auto r_dot_v = dot(reflect_v, eye);

        if (r_dot_v >= 0) {
            auto factor = std::pow(r_dot_v, obj.material().shininess);
            specular = light.intensity * obj.material().specular * factor;
        }
    }

    // XXX reflection hack
    return (ambient + specular + diffuse) * reflect_factor;
}

#endif