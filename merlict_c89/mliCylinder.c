/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCylinder.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliCylinder_is_equal(const struct mliCylinder a, const struct mliCylinder b)
{
        if (a.radius != b.radius)
                return 0;
        if (a.length != b.length)
                return 0;
        return 1;
}
