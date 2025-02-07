/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "string_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"

chk_rc mli_String_to_io(const struct mli_String *self, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_String"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk_IO_write(&self->size, sizeof(uint64_t), 1u, f);
        chk_IO_write(self->array, sizeof(char), self->size, f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_String_from_io(struct mli_String *self, struct mli_IO *f)
{
        uint64_t size;
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_String"));
        mli_MagicId_warn_version(&magic);
        chk_IO_read(&size, sizeof(uint64_t), 1u, f);
        chk(mli_String_malloc(self, size));
        self->size = size;
        chk_IO_read(self->array, sizeof(char), self->size, f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
