/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIINTERSECTION_H_
#define MERLICT_MLIINTERSECTION_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"

#define MLI_VOID_FACE -1

typedef struct {
    uint32_t object_idx;
    mliVec position;
    mliVec surface_normal;
    float distance_of_ray;
    int32_t from_outside_to_inside;
} mliIntersection;

#endif
