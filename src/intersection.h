#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "object.h"
#include "ray.h"

struct intersection {
    double t;
    struct object const* obj;

    bool operator==(struct intersection const& rhs) const {
        return t == rhs.t && obj == rhs.obj;
    }

    struct object const& object() const {
        return *(this->obj);
    }

    friend std::ostream& operator<<(std::ostream& os, struct intersection const& i) {
        os << "{" << i.t << ", " << i.object() << "}";
        return os;
    }
};

struct intersection intersection(double t, struct object const& object) {
    return {t, &object};
}

struct intersections {
    size_t count = 0;
    std::vector<struct intersection> xs;

    struct intersection const& operator[](size_t i) const {
        return xs[i];
    }

    void sort() {
        std::sort(xs.begin(), xs.end(), [](auto a, auto b) { return a.t < b.t;});
    }
};

template<typename... T>
struct intersections intersections(T... t) {
    return {sizeof...(T), std::vector<struct intersection>{t...}};
}

static const struct intersection no_hit {-1, &no_hit_obj};

struct intersection hit(struct intersections const& xs) {
    int res = -1;
    for (int i = 0; i < xs.count; ++i) {
        if (xs[i].t >= 0 && (res == -1 || xs[i].t < xs[res].t)) {
            res = i;
        }
    }
    if (res == -1) return no_hit;
    return xs[res];
}

struct computations {
    double t;
    struct object const* obj;
    struct tuple point;
    struct tuple eye;
    struct tuple normal;
    bool inside;

    struct object const& object() const { return *obj; }
};

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