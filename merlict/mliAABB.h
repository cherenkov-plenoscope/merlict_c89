/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIAABB_H_
#define MLIAABB_H_

#include "mliVec.h"

struct mliAABB {
        /*
         * Rectangular (A)xis-(A)ligned-(B)ounding-(B)ox
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

struct mliAABB mliAABB_set(
        const struct mliVec lower,
        const struct mliVec upper);
struct mliVec mliAABB_center(const struct mliAABB a);
struct mliAABB mliAABB_outermost(
        const struct mliAABB a,
        const struct mliAABB b);
int mliAABB_valid(const struct mliAABB a);
int mliAABB_equal(const struct mliAABB a, const struct mliAABB b);
#endif
