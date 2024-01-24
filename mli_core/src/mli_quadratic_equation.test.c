/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mli_quadratic_equation.h"

CASE("b/a=1., c/a=1., no real solutions")
{
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 1.;
        const double c_over_a = 1.;
        double plus_solution;
        double minus_solution;
        int valid_solutions = mli_quadratic_equation(
                b_over_a, c_over_a, &minus_solution, &plus_solution);
        CHECK(!valid_solutions);
}

CASE("b/a=0., c/a=0., solutions both 0.")
{
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 0.;
        const double c_over_a = 0.;
        double plus_solution;
        double minus_solution;
        CHECK(mli_quadratic_equation(
                b_over_a, c_over_a, &minus_solution, &plus_solution));
        CHECK(minus_solution == 0.);
        CHECK(plus_solution == 0.);
}

CASE("b/a=0., c/a=-1.")
{
        /* ax**2 + bx + c = 0 */
        const double b_over_a = 0.;
        const double c_over_a = -1.;
        double plus_solution;
        double minus_solution;
        CHECK(mli_quadratic_equation(
                b_over_a, c_over_a, &minus_solution, &plus_solution));
        CHECK(minus_solution == -1.);
        CHECK(plus_solution == +1.);
}

CASE("looping combinations")
{
        /* ax**2 + bx + c = 0 */
        double a, b, c;
        for (a = -5.0; a < 5.0; a = a + .25) {
                for (b = -5.0; b < 5.0; b = b + .25) {
                        for (c = -5.0; c < 5.0; c = c + .25) {
                                if (a != 0.0) {
                                        double xp;
                                        double xm;
                                        if (mli_quadratic_equation(
                                                    b / a, c / a, &xm, &xp)) {
                                                CHECK_MARGIN(
                                                        a * xp * xp + b * xp +
                                                                c,
                                                        0.,
                                                        1e-12);
                                                CHECK_MARGIN(
                                                        a * xm * xm + b * xm +
                                                                c,
                                                        0.,
                                                        1e-12);
                                        }
                                }
                        }
                }
        }
}
