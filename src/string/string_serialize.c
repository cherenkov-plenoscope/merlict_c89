/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "string_serialize.h"
#include "../mli/mliMagicId.h"
#include "../chk/chk.h"

int mli_String_fwrite(const struct mli_String *self, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        chk(mliMagicId_set(&magic, "mli_String"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);
        chk_fwrite(&self->size, sizeof(uint64_t), 1u, f);
        chk_fwrite(self->array, sizeof(char), self->size, f);
        return 1;
chk_error:
        return 0;
}

int mli_String_malloc_fread(struct mli_String *self, FILE *f)
{
        uint64_t size;
        struct mliMagicId magic;
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mli_String"));
        mliMagicId_warn_version(&magic);
        chk_fread(&size, sizeof(uint64_t), 1u, f);
        chk(mli_String_malloc(self, size));
        self->size = size;
        chk_fread(self->array, sizeof(char), self->size, f);
        return 1;
chk_error:
        return 0;
}
