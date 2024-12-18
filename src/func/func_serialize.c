/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"

int mli_Func_to_io(const struct mli_Func *func, struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk(mli_MagicId_set(&magic, "mli_Func"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&func->num_points, sizeof(uint64_t), 1u, f);
        chk_IO_write(func->x, sizeof(double), func->num_points, f);
        chk_IO_write(func->y, sizeof(double), func->num_points, f);
        return 1;
chk_error:
        return 0;
}

int mli_Func_from_io(struct mli_Func *func, struct mli_IO *f)
{
        uint64_t num_points;
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Func"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&num_points, sizeof(uint64_t), 1u, f);
        chk(mli_Func_malloc(func, num_points));
        chk_IO_read(func->x, sizeof(double), func->num_points, f);
        chk_IO_read(func->y, sizeof(double), func->num_points, f);
        chk_msg(mli_Func_is_valid(func), "Expected function to be valid.");
        return 1;
chk_error:
        mli_Func_free(func);
        return 0;
}
