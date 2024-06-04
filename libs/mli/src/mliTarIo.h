#ifndef MLITARIO_H_
#define MLITARIO_H_

#include "mliIo.h"
#include "mliTar.h"

int mliTar_read_data_to_io(
        struct mliTar *tar,
        struct mliIo *buff,
        const uint64_t size);
int mliTar_write_data_from_io(
        struct mliTar *tar,
        struct mliIo *buff,
        const uint64_t size);
#endif
