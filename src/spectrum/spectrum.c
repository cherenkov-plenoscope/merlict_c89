/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "spectrum.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "../func/func_serialize.h"
#include "../io/io_text.h"
#include "../magicid/magicid.h"
#include "../string/string_serialize.h"

void mli_Spectrum_free(struct mli_Spectrum *self)
{
        mli_Func_free(&self->spectrum);
        mli_FuncInfo_free(&self->info);
        mli_String_free(&self->name);
}

struct mli_Spectrum mli_Spectrum_init(void)
{
        struct mli_Spectrum out;
        out.spectrum = mli_Func_init();
        out.info = mli_FuncInfo_init();
        out.name = mli_String_init();
        return out;
}

int mli_Spectrum_equal(
        const struct mli_Spectrum *a,
        const struct mli_Spectrum *b)
{
        chk_msg(mli_Func_equal(a->spectrum, b->spectrum),
                "Different spectral function.");
        chk_msg(mli_FuncInfo_equal(&a->info, &b->info),
                "Different spectrum name.");
        chk_msg(mli_String_equal(&a->name, &b->name),
                "Different spectrum name.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Spectrum_to_io(const struct mli_Spectrum *self, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Spectrum"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk(mli_String_to_io(&self->name, f));
        chk(mli_FuncInfo_to_io(&self->info, f));
        chk(mli_Func_to_io(&self->spectrum, f));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Spectrum_from_io(struct mli_Spectrum *self, struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Spectrum"));
        mli_MagicId_warn_version(&magic);

        mli_Spectrum_free(self);
        chk_msg(mli_String_from_io(&self->name, f),
                "Failed to read spectrum name.");
        chk_msg(mli_FuncInfo_from_io(&self->info, f),
                "Failed to read spectrum info.");
        chk_msg(mli_Func_from_io(&self->spectrum, f),
                "Failed to read spectrum.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Spectrum_print_to_io(const struct mli_Spectrum *self, struct mli_IO *f)
{
        uint64_t xamin, xamax, yamin, yamax = 0;
        MLI_MATH_ARRAY_ARGMIN(
                self->spectrum.x, self->spectrum.num_points, xamin);
        MLI_MATH_ARRAY_ARGMAX(
                self->spectrum.x, self->spectrum.num_points, xamax);
        MLI_MATH_ARRAY_ARGMIN(
                self->spectrum.y, self->spectrum.num_points, yamin);
        MLI_MATH_ARRAY_ARGMAX(
                self->spectrum.y, self->spectrum.num_points, yamax);

        chk(mli_IO_text_write_cstr_format(f, "name: '%s', ", self->name.array));
        chk(mli_IO_text_write_cstr_format(
                f, "num. points: %lu\n", self->spectrum.num_points));
        chk(mli_IO_text_write_cstr_format(
                f,
                "x: [%e, %e), '%s'\n",
                self->spectrum.x[xamin],
                self->spectrum.x[xamax],
                self->info.x.array));
        chk(mli_IO_text_write_cstr_format(
                f,
                "y: [%e, %e), '%s'\n",
                self->spectrum.y[yamin],
                self->spectrum.y[yamax],
                self->info.y.array));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
