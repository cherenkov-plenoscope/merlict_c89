/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_SPHERICAL_CAP_H_
#define MERLICT_C89_MLI_SPHERICAL_CAP_H_

#include <math.h>

#include "mliVec.h"
#include "mliRay.h"
#include "mliQuadraticEquation.h"

struct mliVec mli_spherical_cap_surface_normal(
        const struct mliVec intersection_point,
        const double radius);
int mli_spherical_cap_equation(
        const struct mliRay ray,
        const double radius,
        double *plus_solution,
        double *minus_solution);
#endif
