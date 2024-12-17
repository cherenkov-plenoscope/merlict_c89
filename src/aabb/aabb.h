/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_AABB_H_
#define MLI_AABB_H_

#include "../vec/vec.h"

struct mli_AABB {
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
        struct mli_Vec lower;
        struct mli_Vec upper;
};

struct mli_AABB mli_AABB_set(
        const struct mli_Vec lower,
        const struct mli_Vec upper);
struct mli_Vec mli_AABB_center(const struct mli_AABB a);
struct mli_AABB mli_AABB_outermost(
        const struct mli_AABB a,
        const struct mli_AABB b);
int mli_AABB_valid(const struct mli_AABB a);
int mli_AABB_equal(const struct mli_AABB a, const struct mli_AABB b);
int mli_AABB_overlapp_aabb(const struct mli_AABB a, const struct mli_AABB b);
int mli_AABB_is_overlapping(const struct mli_AABB a, const struct mli_AABB b);
int mli_AABB_is_point_inside(
        const struct mli_AABB a,
        const struct mli_Vec point);
#endif
