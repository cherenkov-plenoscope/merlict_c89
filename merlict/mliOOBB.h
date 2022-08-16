/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOOBB_H_
#define MLIOOBB_H_

#include "mliVec.h"
#include "mliAABB.h"
#include "mliHomTra.h"

struct mliOOBB {
        /*
         * (O)bject-(O)riented-(B)ounding-(B)ox OOBB
         * -----------------------------------------
         *
         * In contrast to an (A)xis-(A)ligned-(B)ounding-(B)ox, the OOBB
         * is potentially smaller.
         * Imagine a stick running diagonal to the axes.
         * It has a large AABB, but only a small OOBB.
         *
         *
         *
         *                    (4)--------------------(7)
         *                    /.                     /|
         *                   / .                    / |
         *                  /  .                   /  |
         *                 /   .                  /   |
         *               (5)--------------------(6)   |
         *                |    .                 |    |
         *      Z'        |    .                 |    |
         *      |         |   (0)- - - - - - - - | - (3)
         *      |         |   .                  |   /
         *      |         |  .                   |  /
         *      /-----Y'  | .                    | /
         *     /          |.                     |/
         *    X'         (1)--------------------(2)
         *
         *
         */
        struct mliHomTra root2object;
        struct mliAABB box;
};

int mli_estimate_covariant_transformation_from_vertices(
        const struct mliVec *vertices,
        const uint64_t num_vertices,
        struct mliHomTra *t);

#endif
