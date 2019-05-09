/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOUTERPRISMBOUND_H_
#define MERLICT_MLIOUTERPRISMBOUND_H_

#include <math.h>
#include "mliVec.h"
#include "mliRay.h"


typedef struct {
    double plus_solution;
    double minus_solution;
    int plus_is_inside;
    int minus_is_inside;
} mliBoundSurfaceChecklist;

int mli_outer_bound_surface_causal_intersection(
    const mliBoundSurfaceChecklist cl,
    double *causal_solution) {
    int min_is_causal;
    int plu_is_causal;
    int plu_and_min_are_inside;
    int plu_and_min_are_causal;

    int is_inside_cylinder = 0;

    if (cl.plus_solution > 0.0) {
        plu_is_causal = 1;
    } else {
        plu_is_causal = 0;
    }

    if (cl.minus_solution > 0.0) {
        min_is_causal = 1;
    } else {
        min_is_causal = 0;
    }

    plu_and_min_are_inside = cl.plus_is_inside * cl.minus_is_inside;
    plu_and_min_are_causal = plu_is_causal * min_is_causal;

    if (plu_and_min_are_inside) {
        if (plu_and_min_are_causal) {
            if (cl.minus_solution >= cl.plus_solution) {
                (*causal_solution) = cl.plus_solution;
                is_inside_cylinder = 1;
            } else {
                (*causal_solution) = cl.minus_solution;
                is_inside_cylinder = 1;
            }
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
    } else {
        (*causal_solution) = cl.minus_solution;
    }

    if (is_inside_cylinder) {
        return 1;
    }

    return 0;
}

#endif
