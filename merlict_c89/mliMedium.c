/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium.h"

int mliMedium_equal(const struct mliMedium a, const struct mliMedium b)
{
        if (a.refraction != b.refraction)
                return 0;
        if (a.absorbtion != b.absorbtion)
                return 0;
        return 1;
}
