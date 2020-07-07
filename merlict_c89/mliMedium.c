/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliMedium_is_equal(const struct mliMedium a, const struct mliMedium b)
{
        if (a.refraction != b.refraction)
                return 0;
        if (a.absorbtion != b.absorbtion)
                return 0;
        return 1;
}
