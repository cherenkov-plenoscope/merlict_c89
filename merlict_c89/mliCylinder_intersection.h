/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICYLINDER_INTERSECTION_H_
#define MERLICT_C89_MLICYLINDER_INTERSECTION_H_

#include <math.h>

#include "mli_math.h"
#include "mliIntersection.h"
#include "mliQuadraticEquation.h"

int mliCylinder_intersection(
        const struct mliCylinder cylinder,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray,
        struct mliIntersection *intersection);
void mliCylinder_set_intersection(
        const struct mliHomTra *local2root,
        const struct mliRay *ray_local,
        const double ray_solution,
        struct mliIntersection *intersection);
struct mliVec mliCylinder_surface_normal(const double x, const double y);
int mli_cylinder_equation(
        const double radius,
        const struct mliRay ray,
        double *plus_solution,
        double *minus_solution);
#endif
