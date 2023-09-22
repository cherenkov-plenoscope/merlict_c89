/* Copyright 2018-2020 Sebastian Achim Mueller */

#include "mli_testing.h"
#include "mli_barycentric.h"

struct mliBarycentrigWeights mli_barycentric_weights(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliVec t)
{
        /*
                                         c
                                        /|\
                                       / | \
                                      /  |  \
                                     /   |   \
                                    /    |    \
                                   /     |     \
                                  /      |      \
                                 /       |       \
                                /        |        \
                               /         |         \
                              /          |          \
                             /           |           \
                            /            |            \
                           /             |             \
                         ac              |              \
                         /               |               \
                        /                |                \
                       /      atc        |                 \
                      /                  |                  \
                     /                   t                   \
                    /                   /  \                  \
                   /                 /        \                \
                  /               /              \              \
                 /             /                    \            \
                /           at                         \          \
               /         /                                \        \
              /       /                                      \      \
             /     /                    abt                     \    \
            /   /                                                  \  \
           / /                                                        \\
          a------------------------------ab-----------------------------b

        */
        struct mliBarycentrigWeights weights;

        const struct mliVec ab = mliVec_substract(b, a);
        const struct mliVec ac = mliVec_substract(c, a);

        const double ab_ab = mliVec_dot(ab, ab);
        const double ab_ac = mliVec_dot(ab, ac);
        const double ac_ac = mliVec_dot(ac, ac);

        const double abc_area_pow2 = ab_ab * ac_ac - ab_ac * ab_ac;

        const struct mliVec at = mliVec_substract(t, a);
        const double at_ab = mliVec_dot(at, ab);
        const double at_ac = mliVec_dot(at, ac);

        const double atc_area_pow2 = ab_ab * at_ac - ab_ac * at_ab;
        const double abt_area_pow2 = ac_ac * at_ab - ab_ac * at_ac;

        weights.c = atc_area_pow2 / abc_area_pow2;
        weights.b = abt_area_pow2 / abc_area_pow2;
        weights.a = 1.0 - weights.b - weights.c;

        return weights;
}
