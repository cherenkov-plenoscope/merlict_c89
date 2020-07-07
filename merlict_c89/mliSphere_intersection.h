/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERE_INTERSECTION_H_
#define MERLICT_C89_MLISPHERE_INTERSECTION_H_

#include <math.h>

#include "mliSphere.h"
#include "mli_math.h"
#include "mliOBB.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"
#include "mli_from_outside_to_inside.h"

int mliSphere_intersection(
        const struct mliSphere sphere,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection);
void mliSphere_set_intersection(
        const struct mliHomTra *local2root,
        const struct mliRay *ray_local,
        const double ray_solution,
        struct mliIntersection *intersection);
int mli_sphere_intersection_equation(
        const double radius,
        const struct mliRay ray,
        double *plus_solution,
        double *minus_solution);
#endif
