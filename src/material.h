#ifndef MATERIAL_H
#define MATERIAL_H

#include "tuple.h"
#include "light.h"

struct material {
    struct tuple color{1, 1, 1, 0};
    double ambient = 0.1;
    double diffuse = 0.9;
    double specular = 0.9;
    double shininess = 200;

    bool operator==(struct material const& rhs) const {
        return color == rhs.color &&
               ambient == rhs.ambient &&
               diffuse == rhs.diffuse &&
               specular == rhs.specular &&
               shininess == rhs.shininess;
    }

    friend std::ostream& operator<<(std::ostream& os, struct material const& m) {
        os << "{" << m.color << ", " << m.ambient << ", " << m.diffuse << ", " << m.specular << ", " << m.shininess << "}";
        return os;
    }
};

struct tuple lighting(struct material const& material, struct light const& light, struct tuple point, struct tuple eye, struct tuple normal) {
    auto base_color = material.color * light.intensity;
    auto ambient = base_color * material.ambient;
    auto diffuse = color(0, 0, 0);
    auto specular = color(0, 0, 0);

    auto light_v = normalize(light.position - point);
    auto l_dot_n = dot(light_v, normal);

    if (l_dot_n >= 0) {
        diffuse = base_color * material.diffuse * l_dot_n;

        auto reflect_v = reflect(-light_v, normal);
        auto r_dot_v = dot(reflect_v, eye);

        if (r_dot_v >= 0) {
            auto factor = std::pow(r_dot_v, material.shininess);
            specular = light.intensity * material.specular * factor;
        }
    }

    return ambient + diffuse + specular;
}

#endif