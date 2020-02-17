/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDRICAL_PRISM_H_
#define MERLICT_MLICYLINDRICAL_PRISM_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"


int mli_inside_cylindrical_prism(
    const struct mliVec v,
    const double radius) {
    const double distance_sq = v.x*v.x + v.y*v.y;
    if (distance_sq <= radius*radius) {
        return 1;
    } else {
        return 0;
    }
}

#endif
