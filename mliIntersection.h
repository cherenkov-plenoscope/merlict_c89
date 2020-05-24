/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIINTERSECTION_H_
#define MERLICT_MLIINTERSECTION_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"

struct mliIntersection {
        uint64_t object_idx;
        struct mliVec position;
        struct mliVec surface_normal;
        struct mliVec position_local;
        struct mliVec surface_normal_local;
        double distance_of_ray;
        int from_outside_to_inside;
};

#endif
