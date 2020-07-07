/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliHexagon.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliHexagon_is_equal(const struct mliHexagon a, const struct mliHexagon b)
{
        return a.inner_radius == b.inner_radius;
}
