#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "object.h"

struct intersection {
    double t;
    struct object const& object;

    bool operator==(struct intersection const& rhs) const {
        return t == rhs.t && object == rhs.object;
    }

    friend std::ostream& operator<<(std::ostream& os, struct intersection const& i) {
        os << "{" << i.t << ", " << i.object << "}";
        return os;
    }
};

struct intersection intersection(double t, struct object const& object) {
    return {t, object};
}

struct intersections {
    size_t count;
    std::vector<struct intersection> xs;

    struct intersection const& operator[](size_t i) const {
        return xs[i];
    }
};

template<typename... T>
struct intersections intersections(T... t) {
    return {sizeof...(T), std::vector<struct intersection>{t...}};
}

static const struct object no_hit_obj{identity_matrix};
static const struct intersection no_hit {-1, no_hit_obj};

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

#endif