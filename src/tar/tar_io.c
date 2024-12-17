/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "tar_io.h"
#include <stdio.h>
#include "../chk/chk.h"
#include "../io/io_memory.h"

int mli_Tar_read_data_to_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size)
{
        uint64_t i;
        chk_msg(buff->cstr != NULL, "buff is not allocated.");
        for (i = 0; i < size; i++) {
                unsigned char c;
                chk(mli_Tar_read_data(tar, (void *)(&c), 1));
                chk(mli_IO_write(buff, (void *)(&c), sizeof(unsigned char), 1));
        }

        return 1;
chk_error:
        return 0;
}

int mli_Tar_write_data_from_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size)
{
        uint64_t i;
        chk_msg(tar->stream, "tar is not open.");
        for (i = 0; i < size; i++) {
                unsigned char c;
                chk(mli_IO_read(buff, (void *)(&c), sizeof(unsigned char), 1));
                chk(mli_Tar_write_data(tar, (void *)(&c), 1));
        }

        return 1;
chk_error:
        return 0;
}
