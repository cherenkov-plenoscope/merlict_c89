/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc_serialize.h"
#include "mliMagicId.h"

int mliFunc_fwrite(const struct mliFunc *func, FILE *f)
{
        struct mliMagicId magic;
        mli_c(mliMagicId_set(&magic, "mliFunc"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        mli_fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
        mli_fwrite(func->x, sizeof(double), func->num_points, f);
        mli_fwrite(func->y, sizeof(double), func->num_points, f);
        return 1;
error:
        return 0;
}

int mliFunc_malloc_fread(struct mliFunc *func, FILE *f)
{
        uint32_t num_points;
        struct mliMagicId magic;
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliFunc"));
        mliMagicId_warn_version(&magic);

        mli_fread(&num_points, sizeof(uint32_t), 1u, f);
        mli_c(mliFunc_malloc(func, num_points));
        mli_fread(func->x, sizeof(double), func->num_points, f);
        mli_fread(func->y, sizeof(double), func->num_points, f);
        mli_check(
                mliFunc_x_is_strictly_increasing(func),
                "Expected function x-arguments to be ascending, but they are "
                "not.");
        return 1;
error:
        mliFunc_free(func);
        return 0;
}
