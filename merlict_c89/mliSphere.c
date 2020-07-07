/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSphere.h"

int mliSphere_is_equal(const struct mliSphere a, const struct mliSphere b)
{
        return a.radius == b.radius;
}
