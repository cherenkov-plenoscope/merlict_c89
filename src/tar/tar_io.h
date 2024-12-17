#ifndef MLITARIO_H_
#define MLITARIO_H_

#include "../io/io_memory.h"
#include "tar.h"

int mli_Tar_read_data_to_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size);
int mli_Tar_write_data_from_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size);
#endif
