#ifndef MATERIAL_H
#define MATERIAL_H

#include "tuple.h"
#include "light.h"

struct material {
    double ambient = 0.1;
    double diffuse = 0.9;
    double specular = 0.9;
    double shininess = 200;

    virtual ~material() {}

    virtual bool operator==(struct material const& rhs) const {
        return ambient == rhs.ambient &&
               diffuse == rhs.diffuse &&
               specular == rhs.specular &&
               shininess == rhs.shininess;
    }

    virtual struct tuple color_at(struct tuple const& p) const = 0;

    virtual void print(std::ostream& os) const {
        os <<  ambient << ", " << diffuse << ", " << specular << ", " << shininess;
    }

    friend std::ostream& operator<<(std::ostream& os, struct material const& m) {
        m.print(os);
        return os;
    }
};

struct color_material : public material {
    struct tuple color{1, 1, 1, 0};

    color_material() {}
    color_material(double r, double g, double b) : color(::color(r, g, b)) {}

    virtual bool operator==(struct material const& other) const override {
        try {
            auto rhs = dynamic_cast<struct color_material const&>(other);
            return color == rhs.color &&
                this->material::operator==(rhs);
        } catch (std::bad_cast) {
            return false;
        }
    }

    virtual struct tuple color_at(struct tuple const& p) const override {
        return color;
    }

    virtual void print(std::ostream& os) const override {
        os << "{" << color << ", ";
        material::print(os);
        os << "}";
    }
};

struct tuple lighting(struct material const& material, struct light const& light, struct tuple point, struct tuple eye, struct tuple normal) {
    auto base_color = material.color_at(point) * light.intensity;
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