/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func_serialize.h"
#include "../mli/mliMagicId.h"
#include "../chk/chk.h"

int mli_Func_fwrite(const struct mli_Func *func, FILE *f)
{
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mli_Func"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
        chk_fwrite(func->x, sizeof(double), func->num_points, f);
        chk_fwrite(func->y, sizeof(double), func->num_points, f);
        return 1;
chk_error:
        return 0;
}

int mli_Func_malloc_fread(struct mli_Func *func, FILE *f)
{
        uint32_t num_points;
        struct mliMagicId magic;
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mli_Func"));
        mliMagicId_warn_version(&magic);

        chk_fread(&num_points, sizeof(uint32_t), 1u, f);
        chk(mli_Func_malloc(func, num_points));
        chk_fread(func->x, sizeof(double), func->num_points, f);
        chk_fread(func->y, sizeof(double), func->num_points, f);
        chk_msg(mli_Func_is_valid(func), "Expected function to be valid.");
        return 1;
chk_error:
        mli_Func_free(func);
        return 0;
}
