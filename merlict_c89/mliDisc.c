/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDisc.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliDisc_is_equal(const struct mliDisc a, const struct mliDisc b)
{
        return a.radius == b.radius;
}

int mliDisc_is_valid(const struct mliDisc a) { return a.radius >= 0.; }
