/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLITARIO_H_
#define MLITARIO_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../io/io.h"
#include "tar.h"

chk_rc mli_Tar_read_data_to_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size);
chk_rc mli_Tar_write_data_from_IO(
        struct mli_Tar *tar,
        struct mli_IO *buff,
        const uint64_t size);
#endif
