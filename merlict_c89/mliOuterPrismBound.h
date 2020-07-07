/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOUTERPRISMBOUND_H_
#define MERLICT_C89_MLIOUTERPRISMBOUND_H_

#include <math.h>

#include "mliVec.h"
#include "mliRay.h"

struct mliBoundSurfaceChecklist {
        double plus_solution;
        double minus_solution;
        int plus_is_inside;
        int minus_is_inside;
};

int mli_outer_bound_surface_causal_intersection(
        const struct mliBoundSurfaceChecklist cl,
        double *causal_solution);
#endif
