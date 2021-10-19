#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include "intersection.h"
#include "light.h"
#include "object.h"

struct world {
    std::vector<struct light> lights_container;
    std::vector<std::unique_ptr<object>> objects_container;

    void light(struct light l) {
        lights_container.emplace_back(l);
    }

    template<typename ObjType, typename... Args>
    struct object& object(Args... args) {
        return *objects_container.emplace_back(std::make_unique<ObjType>(std::forward<Args>(args)...));
    }

    struct light& lights(size_t i) {
        return lights_container[i];
    }

    struct light const& lights(size_t i) const {
        return lights_container[i];
    }

    struct object& objects(size_t i) {
        return *objects_container[i];
    }
};

struct intersections intersects(struct world const& w, struct ray const& r) {
    struct intersections xs;
    for (auto& obj : w.objects_container) obj->intersects_in(r, xs);
    xs.sort();
    return xs;
}

struct tuple color_at(struct world const&, struct ray const&, size_t);
struct tuple reflected_color(struct world const& w, struct computations const& comps, size_t depth = 0) {
    if (depth > 3 || comps.object().material().reflective == 0) return color(0, 0, 0);
    else {
        auto r = ray(comps.over_point, comps.reflect);
        auto c = color_at(w, r, depth + 1);
        return c * comps.object().material().reflective;
    }
}

bool is_shadowed(struct world const& world, struct light const& light, struct tuple const& point) {
    auto l = light.position - point;
    auto h = hit(intersects(world, ray(point + normalize(l) * 0.01, normalize(l))));
    if (h != no_hit && h.t < magnitude(l)) return true;
    return false;
}

struct tuple shade_hit(struct world const& world, struct computations const& comps, size_t depth = 0) {
    auto res = color(0, 0, 0);
    for (auto const& light : world.lights_container) {
        res += lighting(comps.object(), light, comps.point, comps.eye, comps.normal, is_shadowed(world, light, comps.point));
    }
    res += reflected_color(world, comps, depth);
    return res;
}

struct tuple color_at(struct world const& world, struct ray const& r, size_t depth = 0) {
    auto xs = intersects(world, r);
    auto h = hit(xs);
    if (h == no_hit) return color(0, 0, 0);
    auto comps = prepare_computations(h, r);
    return shade_hit(world, comps, depth);
}

#endif