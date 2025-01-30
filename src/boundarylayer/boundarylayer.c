/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "boundarylayer.h"
#include <stdio.h>

int mli_BoundaryLayer_equal(
        const struct mli_BoundaryLayer a,
        const struct mli_BoundaryLayer b)
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

void mli_BoundaryLayer_print(const struct mli_BoundaryLayer a)
{
        fprintf(stderr,
                "inner %d srf / %d med\nouter %d srf / %d med\n",
                a.inner.surface,
                a.inner.medium,
                a.outer.surface,
                a.outer.medium);
}

void mli_BoundaryLayer2_free(struct mli_BoundaryLayer2 *self)
{
        mli_String_free(&self->name);
}

struct mli_BoundaryLayer2 mli_BoundaryLayer2_init(void)
{
        struct mli_BoundaryLayer2 layer;
        layer.inner.medium = mli_BoundaryLayer_Medium_init();
        layer.inner.surface = mli_BoundaryLayer_Surface_init();
        layer.outer.medium = mli_BoundaryLayer_Medium_init();
        layer.outer.surface = mli_BoundaryLayer_Surface_init();
        layer.name = mli_String_init();
        return layer;
}
