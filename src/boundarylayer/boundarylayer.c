/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "boundarylayer.h"
#include <stdio.h>
#include "../magicid/magicid.h"
#include "../string/string_serialize.h"

void mli_BoundaryLayer_free(struct mli_BoundaryLayer *self)
{
        mli_String_free(&self->name);
}

struct mli_BoundaryLayer mli_BoundaryLayer_init(void)
{
        struct mli_BoundaryLayer layer;
        layer.inner.medium = 0;
        layer.inner.surface = 0;
        layer.outer.medium = 0;
        layer.outer.surface = 0;
        layer.name = mli_String_init();
        return layer;
}

mli_bool mli_BoundaryLayer_equal(
        const struct mli_BoundaryLayer *a,
        const struct mli_BoundaryLayer *b)
{
        if (!mli_String_equal(&a->name, &b->name)) {
                return MLI_FALSE;
        }
        if (a->inner.surface != b->inner.surface) {
                return MLI_FALSE;
        }
        if (a->inner.medium != b->inner.medium) {
                return MLI_FALSE;
        }
        if (a->outer.surface != b->outer.surface) {
                return MLI_FALSE;
        }
        if (a->outer.medium != b->outer.medium) {
                return MLI_FALSE;
        }
        return MLI_TRUE;
}

chk_rc mli_BoundaryLayer_to_io(
        const struct mli_BoundaryLayer *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_msg(mli_String_to_io(&self->name, f),
                "Can't write boundary layer name to io.");
        chk_IO_write(&self->inner.medium, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->inner.surface, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->outer.medium, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->outer.surface, sizeof(uint64_t), 1u, f);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_BoundaryLayer_from_io(
        struct mli_BoundaryLayer *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer"));
        mli_MagicId_warn_version(&magic);

        chk_msg(mli_String_from_io(&self->name, f),
                "Can't read boundary layer name from io.");
        chk_IO_read(&self->inner.medium, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->inner.surface, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->outer.medium, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->outer.surface, sizeof(uint64_t), 1u, f);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
