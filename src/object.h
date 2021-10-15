#ifndef OBJECT_H
#define OBJECT_H

#include "matrix.h"
#include "material.h"

struct object {
    struct matrix_t<4> transform;
    struct material material;

    object(struct matrix_t<4> t) : transform(t) {}

    virtual ~object() {}

    virtual bool operator==(struct object const& rhs) const { 
        return transform == rhs.transform && material == rhs.material; 
    }

    friend std::ostream& operator<<(std::ostream& os, struct object const& o) {
        os << "{object}";
        return os;
    }
};

#endif