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

static const size_t max_ray_depth = 6;

struct tuple color_at(struct world const&, struct ray const&, size_t);
struct tuple reflected_color(struct world const& w, struct computations const& comps, size_t depth = 0) {
    if (depth > max_ray_depth || comps.object().material().reflective == 0) return color(0, 0, 0);
    else {
        auto r = ray(comps.over_point, comps.reflect);
        auto c = color_at(w, r, depth + 1);
        return c * comps.object().material().reflective;
    }
}

struct tuple refracted_color(struct world const& w, struct computations const& comps, size_t depth = 0) {
    if (depth > max_ray_depth || comps.object().material().transparency == 0) return color(0, 0, 0);

    auto n_ratio = comps.n1 / comps.n2;
    auto cos_i = dot(comps.eye, comps.normal);
    auto sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
    if (sin2_t > 1) return color(0, 0, 0); // T O T A L   I N T E R N A L   R E F L E C T I O N

    auto cos_t = sqrt(1 - sin2_t);
    auto direction = comps.normal * (n_ratio * cos_i - cos_t) - comps.eye * n_ratio;
    auto r = ray(comps.under_point, direction);
    auto c = color_at(w, r, depth + 1);
    return c * comps.object().material().transparency; 

    return color(1, 1, 1);
}

bool is_shadowed(struct world const& world, struct light const& light, struct tuple const& point) {
    auto l = light.position - point;
    auto h = hit(intersects(world, ray(point + normalize(l) * 0.01, normalize(l))));
    if (h != no_hit && h.t < magnitude(l)) return true;
    return false;
}

struct tuple shade_hit(struct world const& world, struct computations const& comps, size_t depth = 0) {
    auto surface = color(0, 0, 0);
    for (auto const& light : world.lights_container) {
        surface += lighting(comps.object(), light, comps.point, comps.eye, comps.normal, is_shadowed(world, light, comps.point));
    }
    auto reflected = reflected_color(world, comps, depth);
    auto refracted = refracted_color(world, comps, depth);

    auto& material = comps.object().material();
    if (material.reflective > 0 && material.transparency > 0) {
        auto reflectance = schlick(comps);
        return surface + reflected * reflectance + refracted * (1 - reflectance);
    } else {
        return surface + reflected + refracted;
    }
}

struct tuple color_at(struct world const& world, struct ray const& r, size_t depth = 0) {
    auto xs = intersects(world, r);
    auto h = hit(xs);
    if (h == no_hit) return color(0, 0, 0);
    auto comps = prepare_computations(h, r, xs);
    return shade_hit(world, comps, depth);
}

#endif