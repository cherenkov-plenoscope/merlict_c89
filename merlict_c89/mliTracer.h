/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRACER_H_
#define MERLICT_C89_MLITRACER_H_

#include <math.h>
#include <stdint.h>

#include "mliCombine.h"
#include "mliIntersection.h"
#include "mli_ray_octree_traversal.h"

struct mliColor mli_trace(
        const struct mliCombine *combine,
        const struct mliRay ray);
int mli_first_casual_intersection(
        const struct mliCombine *combine,
        const struct mliRay ray,
        struct mliIntersection *intersection);
#endif
