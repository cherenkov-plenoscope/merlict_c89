/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliBoundaryLayer.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliBoundaryLayer_is_equal(
        const struct mliBoundaryLayer a,
        const struct mliBoundaryLayer b)
{
        if (a.inner.surface != b.inner.surface)
                return 0;
        if (a.outer.surface != b.outer.surface)
                return 0;
        if (a.inner.medium != b.inner.medium)
                return 0;
        if (a.outer.medium != b.outer.medium)
                return 0;
        return 1;
}
