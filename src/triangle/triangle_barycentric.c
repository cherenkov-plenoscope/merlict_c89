/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "triangle_barycentric.h"

struct mli_triangle_BarycentrigWeights mli_triangle_barycentric_weights(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_Vec t)
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
        struct mli_triangle_BarycentrigWeights weights;

        const struct mli_Vec ab = mli_Vec_substract(b, a);
        const struct mli_Vec ac = mli_Vec_substract(c, a);

        const double ab_ab = mli_Vec_dot(ab, ab);
        const double ab_ac = mli_Vec_dot(ab, ac);
        const double ac_ac = mli_Vec_dot(ac, ac);

        const double abc_area_pow2 = ab_ab * ac_ac - ab_ac * ab_ac;

        const struct mli_Vec at = mli_Vec_substract(t, a);
        const double at_ab = mli_Vec_dot(at, ab);
        const double at_ac = mli_Vec_dot(at, ac);

        const double atc_area_pow2 = ab_ab * at_ac - ab_ac * at_ab;
        const double abt_area_pow2 = ac_ac * at_ab - ab_ac * at_ac;

        weights.c = atc_area_pow2 / abc_area_pow2;
        weights.b = abt_area_pow2 / abc_area_pow2;
        weights.a = 1.0 - weights.b - weights.c;

        return weights;
}
