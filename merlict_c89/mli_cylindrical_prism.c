/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_cylindrical_prism.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mli_inside_cylindrical_prism(const struct mliVec v, const double radius)
{
        const double distance_sq = v.x * v.x + v.y * v.y;
        if (distance_sq <= radius * radius) {
                return 1;
        } else {
                return 0;
        }
}
