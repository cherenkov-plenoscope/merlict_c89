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

int mli_BoundaryLayer2_equal_physics(
        const struct mli_BoundaryLayer2 *a,
        const struct mli_BoundaryLayer2 *b)
{
        if (!mli_BoundaryLayer_Surface_equal(
                    &a->inner.surface, &b->inner.surface)) {
                return 0;
        }
        if (!mli_BoundaryLayer_Medium_equal(
                    &a->inner.medium, &b->inner.medium)) {
                return 0;
        }
        if (!mli_BoundaryLayer_Surface_equal(
                    &a->outer.surface, &b->outer.surface)) {
                return 0;
        }
        if (!mli_BoundaryLayer_Medium_equal(
                    &a->outer.medium, &b->outer.medium)) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer2_to_io(
        const struct mli_BoundaryLayer2 *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer2"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_msg(mli_BoundaryLayer_Medium_to_io(&self->inner.medium, f),
                "Can't write inner.medium to io.");
        chk_msg(mli_BoundaryLayer_Surface_to_io(&self->inner.surface, f),
                "Can't write inner.surface to io.");
        chk_msg(mli_BoundaryLayer_Medium_to_io(&self->outer.medium, f),
                "Can't write outer.medium to io.");
        chk_msg(mli_BoundaryLayer_Surface_to_io(&self->outer.surface, f),
                "Can't write outer.surface to io.");

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer2_from_io(
        struct mli_BoundaryLayer2 *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer2"));
        mli_MagicId_warn_version(&magic);

        chk_msg(mli_BoundaryLayer_Medium_from_io(&self->inner.medium, f),
                "Can't read inner.medium from io.");
        chk_msg(mli_BoundaryLayer_Surface_from_io(&self->inner.surface, f),
                "Can't read inner.surface from io.");
        chk_msg(mli_BoundaryLayer_Medium_from_io(&self->outer.medium, f),
                "Can't read outer.medium from io.");
        chk_msg(mli_BoundaryLayer_Surface_from_io(&self->outer.surface, f),
                "Can't read outer.surface from io.");

        return 1;
chk_error:
        return 0;
}
