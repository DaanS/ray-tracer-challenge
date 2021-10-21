#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "ray.h"

struct object;
std::ostream& operator<<(std::ostream&, struct object const&);

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

    void add(struct intersection const& i) {
        ++count;
        xs.push_back(i);
    }

    void sort() {
        std::sort(xs.begin(), xs.end(), [](auto a, auto b) { return a.t < b.t;});
    }
};

template<typename... T>
struct intersections intersections(T... t) {
    return {sizeof...(T), std::vector<struct intersection>{t...}};
}

static const struct intersection no_hit {-1, nullptr};

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
    struct tuple over_point;
    struct tuple under_point;
    struct tuple eye;
    struct tuple normal;
    struct tuple reflect;
    bool inside;
    double n1;
    double n2;

    struct object const& object() const { return *obj; }
};

double schlick(struct computations const& comps) {
    auto cos = dot(comps.eye, comps.normal);

    if (comps.n1 > comps.n2) {
        auto n = comps.n1 / comps.n2;
        auto sin2_t = n * n * (1 - cos * cos);
        if (sin2_t > 1) return 1;

        cos = sqrt(1 - sin2_t);
    }

    auto sqrt_r0 = (comps.n1 - comps.n2) / (comps.n1 + comps.n2);
    auto r0 = sqrt_r0 * sqrt_r0;

    return r0 + (1 - r0) * std::pow(1 - cos, 5);
}

#endif