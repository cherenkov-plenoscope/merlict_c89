/* Copyright 2019 Sebastian A. Mueller */
#include "math_quadratic_equation.h"
#include <math.h>

int mli_math_quadratic_equation(
        const double p,
        const double q,
        double *minus_solution,
        double *plus_solution)
{
        /*
         *  y = a*x^2 + b*x + c
         *  p = b/a
         *  q = c/a
         *  x_m = -p/2 - sqrt((-p/2)^2 - q)
         *  x_p = -p/2 + sqrt((-p/2)^2 - q)
         */
        const double p_over_2 = 0.5 * p;
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
