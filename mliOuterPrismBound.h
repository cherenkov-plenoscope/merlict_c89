/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOUTERPRISMBOUND_H_
#define MERLICT_MLIOUTERPRISMBOUND_H_

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
        double *causal_solution)
{
        int min_is_causal;
        int plu_is_causal;
        int plu_and_min_are_inside;
        int plu_and_min_are_causal;

        int is_inside_cylinder = 0;

        plu_is_causal = cl.plus_solution > 0. ? 1 : 0;
        min_is_causal = cl.minus_solution > 0. ? 1 : 0;

        plu_and_min_are_inside = cl.plus_is_inside * cl.minus_is_inside;
        plu_and_min_are_causal = plu_is_causal * min_is_causal;

        if (plu_and_min_are_inside) {
                if (plu_and_min_are_causal) {
                        (*causal_solution) = cl.minus_solution;
                        is_inside_cylinder = 1;
                } else if (plu_is_causal) {
                        (*causal_solution) = cl.plus_solution;
                        is_inside_cylinder = 1;
                } else if (min_is_causal) {
                        (*causal_solution) = cl.minus_solution;
                        is_inside_cylinder = 1;
                }
        } else if (cl.plus_is_inside && plu_is_causal) {
                (*causal_solution) = cl.plus_solution;
                is_inside_cylinder = 1;
        } else if (cl.minus_is_inside && min_is_causal) {
                (*causal_solution) = cl.minus_solution;
                is_inside_cylinder = 1;
        }

        if (is_inside_cylinder) {
                return 1;
        }

        return 0;
}

#endif
