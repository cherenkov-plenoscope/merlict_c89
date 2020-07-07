/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIQUADRATICEQUATION_H_
#define MERLICT_C89_MLIQUADRATICEQUATION_H_

#include <math.h>

int mli_quadratiq_equation(
        const double p,
        const double q,
        double *plus_solution,
        double *minus_solution);
#endif
