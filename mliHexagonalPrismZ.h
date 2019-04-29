/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHEXAGONAL_PRISMZ_H_
#define MERLICT_MLIHEXAGONAL_PRISMZ_H_

#include <math.h>
#include "mliVec.h"
#include "mliMath.h"


int mli_inside_hexagonal_prism_z(
    const mliVec *v,
    const double inner_radius) {
    const mliVec MLIVEC_UNIT_U = {0., 1., 0.,};
    const mliVec MLIVEC_UNIT_V = { 0.8660254037844387, -0.5, 0.,};
    const mliVec MLIVEC_UNIT_W = {-0.8660254037844387, -0.5, 0.,};

    const double projection_onto_UNIT_U = mliVec_dot(&MLIVEC_UNIT_U, v);
    const double projection_onto_UNIT_V = mliVec_dot(&MLIVEC_UNIT_V, v);
    const double projection_onto_UNIT_W = mliVec_dot(&MLIVEC_UNIT_W, v);
    if (
        projection_onto_UNIT_U < inner_radius
        &&
        projection_onto_UNIT_U > -inner_radius
        &&
        projection_onto_UNIT_V < inner_radius
        &&
        projection_onto_UNIT_V > -inner_radius
        &&
        projection_onto_UNIT_W < inner_radius
        &&
        projection_onto_UNIT_W > -inner_radius
    ) {
        return 1;
    } else {
        return 0;
    }
}


#endif
