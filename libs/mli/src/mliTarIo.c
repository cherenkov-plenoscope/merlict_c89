/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "mliTarIo.h"
#include <stdio.h>
#include "chk.h"

int mliTar_read_data_to_io(
        struct mliTar *tar,
        struct mliIo *buff,
        const uint64_t size)
{
        uint64_t i;
        chk_msg(buff->cstr != NULL, "buff is not allocated.");
        for (i = 0; i < size; i++) {
                unsigned char c;
                chk(mliTar_read_data(tar, (void *)(&c), 1));
                chk(mliIo_write_unsigned_char(buff, c));
        }

        return 1;
chk_error:
        return 0;
}

int mliTar_write_data_from_io(
        struct mliTar *tar,
        struct mliIo *buff,
        const uint64_t size)
{
        uint64_t i;
        chk_msg(tar->stream, "tar is not open.");
        for (i = 0; i < size; i++) {
                int rc = mliIo_read_char(buff);
                unsigned char c;
                chk(rc != EOF);
                c = (char)(rc);
                chk(mliTar_write_data(tar, (void *)(&c), 1));
        }

        return 1;
chk_error:
        return 0;
}
