/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBB_H_
#define MLIOBB_H_

#include "mliVec.h"

struct mliOBB {
        /*
         * Rectangular Oriented-Bounding-Box
         * oriented w.r.t. the unit-vectors.
         *
         *                     O----------------------O
         *                    /.                     /|
         *                   / .                    / |
         *                  /  .                   /  |
         *                 /   .                  /   |
         *                O----------------------O upper
         *                |    .                 |    |
         *      Z         |    .                 |    |
         *      |       lo|wer O- - - - - - - - -| - -O
         *      |         |   .                  |   /
         *      |         |  .                   |  /
         *      /-----Y   | .                    | /
         *     /          |.                     |/
         *    X           O----------------------O
         *
         *
         */
        struct mliVec lower;
        struct mliVec upper;
};

struct mliOBB mliOBB_set(const struct mliVec lower, const struct mliVec upper);
struct mliVec mliOBB_center(const struct mliOBB a);
struct mliOBB mliOBB_outermost(const struct mliOBB a, const struct mliOBB b);
int mliOBB_valid(const struct mliOBB obb);
int mliOBB_equal(const struct mliOBB a, const struct mliOBB b);
#endif
