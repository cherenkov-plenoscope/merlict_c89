/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliBoundaryLayer.h"

int mliBoundaryLayer_equal(
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

void mliBoundaryLayer_print(const struct mliBoundaryLayer a)
{
        fprintf(stderr,
                "inner %d srf / %d med\nouter %d srf / %d med\n",
                a.inner.surface,
                a.inner.medium,
                a.outer.surface,
                a.outer.medium);
}
