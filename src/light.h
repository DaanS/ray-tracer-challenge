#ifndef LIGHT_H
#define LIGHT_H

#include "tuple.h"

struct light {
    struct tuple position;
    struct tuple intensity;

    bool operator==(struct light rhs) const {
        return position == rhs.position && intensity == rhs.intensity;
    }
};

struct light point_light(struct tuple pos, struct tuple col) {
    return {pos, col};
}

#endif