/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc_serialize.h"
#include "mliMagicId.h"
#include "chk.h"

int mliFunc_fwrite(const struct mliFunc *func, FILE *f)
{
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mliFunc"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
        chk_fwrite(func->x, sizeof(double), func->num_points, f);
        chk_fwrite(func->y, sizeof(double), func->num_points, f);
        return 1;
error:
        return 0;
}

int mliFunc_malloc_fread(struct mliFunc *func, FILE *f)
{
        uint32_t num_points;
        struct mliMagicId magic;
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliFunc"));
        mliMagicId_warn_version(&magic);

        chk_fread(&num_points, sizeof(uint32_t), 1u, f);
        chk(mliFunc_malloc(func, num_points));
        chk_fread(func->x, sizeof(double), func->num_points, f);
        chk_fread(func->y, sizeof(double), func->num_points, f);
        chk_msg(mliFunc_is_valid(func), "Expected function to be valid.");
        return 1;
error:
        mliFunc_free(func);
        return 0;
}
