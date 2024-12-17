/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func_serialize.h"
#include "../mli/mliMagicId.h"
#include "../chk/chk.h"

int mli_Func_fwrite(const struct mli_Func *func, struct mli_IO *f)
{
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mli_Func"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_IO_write(&func->num_points, sizeof(uint32_t), 1u, f);
        chk_IO_write(func->x, sizeof(double), func->num_points, f);
        chk_IO_write(func->y, sizeof(double), func->num_points, f);
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_fread(struct mli_Func *func, struct mli_IO *f)
{
        uint32_t num_points;
        struct mliMagicId magic;
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mli_Func"));
        mliMagicId_warn_version(&magic);

        chk_IO_read(&num_points, sizeof(uint32_t), 1u, f);
        chk(mli_Func_malloc(func, num_points));
        chk_IO_read(func->x, sizeof(double), func->num_points, f);
        chk_IO_read(func->y, sizeof(double), func->num_points, f);
        chk_msg(mli_Func_is_valid(func), "Expected function to be valid.");
        return 1;
chk_error:
        mli_Func_free(func);
        return 0;
}
