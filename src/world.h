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

    template<typename ObjType>
    void object(ObjType obj) {
        objects_container.emplace_back(std::make_unique<ObjType>(std::forward<ObjType>(obj)));
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

struct tuple shade_hit(struct world const& world, struct computations const& comps) {
    auto res = color(0, 0, 0);
    for (auto const& light : world.lights_container) {
        // XXX block light
        auto l = light.position - comps.point;
        auto h = hit(intersects(world, ray(comps.point + normalize(l) * 0.01, normalize(l))));
        if (h != no_hit && h.t < magnitude(l)) continue;

        res += lighting(comps.object().material, light, comps.point, comps.eye, comps.normal);
    }
    return res;
}

struct tuple color_at(struct world const& world, struct ray const& r) {
    auto xs = intersects(world, r);
    auto h = hit(xs);
    if (h == no_hit) return color(0, 0, 0);
    auto comps = prepare_computations(h, r);
    return shade_hit(world, comps);
}

#endif