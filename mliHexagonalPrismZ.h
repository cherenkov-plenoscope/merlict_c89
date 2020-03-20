/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHEXAGONAL_PRISMZ_H_
#define MERLICT_MLIHEXAGONAL_PRISMZ_H_

#include <math.h>
#include <stdint.h>
#include <assert.h>
#include "mliVec.h"
#include "mli_math.h"


int mli_inside_hexagonal_prism_z(
    const struct mliVec v,
    const double inner_radius) {
    const struct mliVec MLIVEC_UNIT_U = {0., 1., 0.,};
    const struct mliVec MLIVEC_UNIT_V = { MLI_SQRT3_OVER_2, -0.5, 0.,};
    const struct mliVec MLIVEC_UNIT_W = {-MLI_SQRT3_OVER_2, -0.5, 0.,};

    const double projection_onto_UNIT_U = mliVec_dot(MLIVEC_UNIT_U, v);
    const double projection_onto_UNIT_V = mliVec_dot(MLIVEC_UNIT_V, v);
    const double projection_onto_UNIT_W = mliVec_dot(MLIVEC_UNIT_W, v);
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

struct mliVec mli_hexagon_corner(const uint64_t corner) {
    /*
     *            /\ y
     *      2 ____|____ 1
     *       /    |    \
     *      /     |     \
     *  __ /______|______\_0_\ x
     *   3 \      |      /   /
     *      \     |     /
     *       \____|____/
     *      4     |    5
     */
    const uint64_t corner_mod = corner % 6;
    switch (corner_mod) {
        case 0u:
            return mliVec_set(1., 0., 0.);
            break;
        case 1u:
            return mliVec_set(.5, MLI_SQRT3_OVER_2, 0.);
            break;
        case 2u:
            return mliVec_set(-.5, MLI_SQRT3_OVER_2, 0.);
            break;
        case 3u:
            return mliVec_set(-1., 0., 0.);
            break;
        case 4u:
            return mliVec_set(-.5, -MLI_SQRT3_OVER_2, 0.);
            break;
        case 5u:
            return mliVec_set(.5, -MLI_SQRT3_OVER_2, 0.);
            break;
        default:
            assert(0);
            break;
    }
}

#endif
