/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRIANGLE_INTERSECTION_H_
#define MERLICT_C89_MLITRIANGLE_INTERSECTION_H_

#include <math.h>

#include "mli_math.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliIntersection.h"
#include "mli_from_outside_to_inside.h"

int mliTriangle_intersection(
        const struct mliVec vertex0,
        const struct mliVec vertex1,
        const struct mliVec vertex2,
        const struct mliRay ray,
        struct mliIntersection *intersection);
struct mliVec mli_triangle_surface_normal(
        const struct mliVec vertex0,
        const struct mliVec vertex1,
        const struct mliVec vertex2);
int mliRay_intersects_triangle(
        const struct mliVec support,
        const struct mliVec direction,
        const struct mliVec vertex0,
        const struct mliVec vertex1,
        const struct mliVec vertex2,
        double *ray_parameter);
#endif
