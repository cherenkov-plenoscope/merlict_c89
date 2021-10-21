/* Copyright 2020 Sebastian A. Mueller */
#ifndef MERLICT_C89_CORSIKA_TARIO_WRITE_H_
#define MERLICT_C89_CORSIKA_TARIO_WRITE_H_

#include "mliTar.h"
#include <stdint.h>

#define MLI_TARIO_CORSIKA_BUNCH_SIZE sizeof(float) * 8
#define MLI_TARIO_CORSIKA_HEADER_SIZE sizeof(float) * 273

struct mliTarIoCherenkovBunchBuffer {
        uint64_t num_bunches_capacity;
        uint64_t num_bunches;
        float *bunches;
};
struct mliTarIoCherenkovBunchBuffer mliTarIoCherenkovBunchBuffer_init(void);
void mliTarIoCherenkovBunchBuffer_free(struct mliTarIoCherenkovBunchBuffer *b);
int mliTarIoCherenkovBunchBuffer_malloc(
        struct mliTarIoCherenkovBunchBuffer *b,
        const uint64_t num_bunches_capacity);

struct mliTarIoWriter {
        struct mliTar tar;
        int event_number;
        int cherenkov_bunch_block_number;
        struct mliTarIoCherenkovBunchBuffer buffer;
};

struct mliTarIoWriter mliTarIoWriter_init(void);
int mliTarIoWriter_open(
        struct mliTarIoWriter *tio,
        const char *path,
        const int num_bunches_capacity);

int mliTarIoWriter_close(struct mliTarIoWriter *tio);
int mliTarIoWriter_add_runh(struct mliTarIoWriter *tio, const float *runh);
int mliTarIoWriter_add_evth(struct mliTarIoWriter *tio, const float *evth);
int mliTarIoWriter_add_cherenkov_bunch(
        struct mliTarIoWriter *tio,
        const float *bunch);

int mliTarIoWriter_finalize_cherenkov_bunch_block(struct mliTarIoWriter *tio);

#endif
