/* Copyright 2019 Sebastian A. Mueller */
#ifndef MLI_MATH_QUADRATIC_EQUATION_H_
#define MLI_MATH_QUADRATIC_EQUATION_H_

#include "../chk/chk.h"

chk_rc mli_math_quadratic_equation(
        const double p,
        const double q,
        double *minus_solution,
        double *plus_solution);

#endif
