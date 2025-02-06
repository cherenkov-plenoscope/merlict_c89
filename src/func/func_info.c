/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "func_info.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../string/string_serialize.h"

struct mli_FuncInfo mli_FuncInfo_init(void)
{
        struct mli_FuncInfo out;
        out.x = mli_String_init();
        out.y = mli_String_init();
        return out;
}

void mli_FuncInfo_free(struct mli_FuncInfo *self)
{
        mli_String_free(&self->x);
        mli_String_free(&self->y);
        (*self) = mli_FuncInfo_init();
}

int mli_FuncInfo_malloc(struct mli_FuncInfo *self)
{
        chk(mli_String_malloc(&self->x, 0u));
        chk(mli_String_malloc(&self->y, 0u));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_FuncInfo_equal(
        const struct mli_FuncInfo *a,
        const struct mli_FuncInfo *b)
{
        if (!mli_String_equal(&a->x, &b->x))
                return MLI_FALSE;
        if (!mli_String_equal(&a->y, &b->y))
                return MLI_FALSE;
        return MLI_TRUE;
}

int mli_FuncInfo_to_io(const struct mli_FuncInfo *self, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_FuncInfo"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_String_to_io(&self->x, f));
        chk(mli_String_to_io(&self->y, f));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_FuncInfo_from_io(struct mli_FuncInfo *self, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_FuncInfo"));
        mli_MagicId_warn_version(&magic);
        chk(mli_String_from_io(&self->x, f));
        chk(mli_String_from_io(&self->y, f));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
