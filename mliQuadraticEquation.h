/* Copyright 2019 Sebastian A. Mueller */
#ifndef MERLICT_MLIQUADRATICEQUATION_H_
#define MERLICT_MLIQUADRATICEQUATION_H_

#include <math.h>

int mli_quadratiq_equation(
    const double p,
    const double q,
    double *plus_solution,
    double *minus_solution) {
    /*
     *  y = a*x^2 + b*x + c
     *  p = b/a
     *  q = c/a
     *  x_m = -p/2 - sqrt((-p/2)^2 - q)
     *  x_p = -p/2 + sqrt((-p/2)^2 - q)
     */
    const double p_over_2 = 0.5*p;
    const double inner_part_of_squareroot = p_over_2 * p_over_2 - q;
    double squareroot;
    if (inner_part_of_squareroot >= 0.0) {
        squareroot = sqrt(inner_part_of_squareroot);
        (*minus_solution) = -p_over_2 - squareroot;
        (*plus_solution) = -p_over_2 + squareroot;
        return 1;
    } else {
        return 0;
    }
}

#endif
