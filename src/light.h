#ifndef LIGHT_H
#define LIGHT_H

#include "tuple.h"

struct light {
    struct tuple position;
    struct tuple intensity;
};

struct light point_light(struct tuple pos, struct tuple col) {
    return {pos, col};
}

#endif