/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_XYPLANE_H_
#define MERLICT_C89_MLI_XYPLANE_H_

#include <math.h>

#include "mliVec.h"
#include "mliRay.h"
#include "mliQuadraticEquation.h"

struct mliVec mli_xyplane_surface_normal(void);
int mli_xyplane_equation(const struct mliRay ray, double *solution);
#endif
