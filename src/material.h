#ifndef MATERIAL_H
#define MATERIAL_H

#include "matrix.h"
#include "tuple.h"
#include "light.h"

struct material {
    double ambient = 0.1;
    double diffuse = 0.9;
    double specular = 0.9;
    double shininess = 200;
    double reflective = 0;
    double transparency = 0;
    double refractive_index = 1;

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

struct stripe {
    struct tuple c1;
    struct tuple c2;

    bool operator==(struct stripe const& rhs) const {
        return c1 == rhs.c1 && c2 == rhs.c2;
    }

    struct tuple operator()(struct tuple const& p) const {
        if (static_cast<int>(std::floor(p.x)) % 2 == 0) return c1;
        else return c2;
    }
};

struct gradient {
    struct tuple c1;
    struct tuple c2;

    bool operator==(struct gradient const& rhs) const {
        return c1 == rhs.c1 && c2 == rhs.c2;
    }

    struct tuple operator()(struct tuple const& p) const {
        return c1 + (c2 - c1) * (p.x - std::floor(p.x));
    }
};

struct ring {
    struct tuple c1;
    struct tuple c2;

    bool operator==(struct ring const& rhs) const {
        return c1 == rhs.c1 && c2 == rhs.c2;
    }

    struct tuple operator()(struct tuple const& p) const {
        if (static_cast<int>(std::floor(std::sqrt(p.x * p.x + p.z * p.z))) % 2 == 0) return c1;
        else return c2;
    }
};

struct checkers {
    struct tuple c1;
    struct tuple c2;

    bool operator==(struct checkers const& rhs) const {
        return c1 == rhs.c1 && c2 == rhs.c2;
    }

    struct tuple operator()(struct tuple const& p) const {
        if (static_cast<int>(std::floor(p.x + epsilon) + std::floor(p.y + epsilon) + std::floor(p.z + epsilon)) % 2 == 0) return c1;
        else return c2;
    }
};

template<typename PatternFunc>
struct pattern_material : public material {
    PatternFunc pattern;
    struct matrix_t<4> transform = identity_matrix;
    struct matrix_t<4> inverse_transform = identity_matrix;

    pattern_material(PatternFunc pattern) : pattern(pattern) {}

    void transformation(struct matrix_t<4> const& m) {
        assert(is_invertible(m));
        transform = m;
        inverse_transform = inverse(m);
    }

    virtual bool operator==(struct material const& other) const override {
        try {
            auto rhs = dynamic_cast<struct pattern_material<PatternFunc> const&>(other);
            return pattern == rhs.pattern &&
                this->material::operator==(rhs);
        } catch (std::bad_cast) {
            return false;
        }
    }

    virtual struct tuple color_at(struct tuple const& p) const override {
        return pattern(inverse_transform * p);
    }

    virtual void print(std::ostream& os) const override {
        material::print(os);
    }
};

template<typename PatternFunc>
struct pattern_material<PatternFunc> pattern(struct tuple const& c1, struct tuple const& c2) {
    return pattern_material<PatternFunc>(PatternFunc{c1, c2});
}

#endif