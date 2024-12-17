/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "string_serialize.h"
#include "../mli/mliMagicId.h"
#include "../chk/chk.h"

int mli_String_fwrite(const struct mli_String *self, struct mli_IO *f)
{
        struct mliMagicId magic = mliMagicId_init();
        chk(mliMagicId_set(&magic, "mli_String"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);
        chk_IO_write(&self->size, sizeof(uint64_t), 1u, f);
        chk_IO_write(self->array, sizeof(char), self->size, f);
        return 1;
chk_error:
        return 0;
}

int mli_String_malloc_fread(struct mli_String *self, struct mli_IO *f)
{
        uint64_t size;
        struct mliMagicId magic;
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mli_String"));
        mliMagicId_warn_version(&magic);
        chk_IO_read(&size, sizeof(uint64_t), 1u, f);
        chk(mli_String_malloc(self, size));
        self->size = size;
        chk_IO_read(self->array, sizeof(char), self->size, f);
        return 1;
chk_error:
        return 0;
}
