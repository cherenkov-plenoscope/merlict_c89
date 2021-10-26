/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_TARIO_WRITE_H_
#define MLI_CORSIKA_TARIO_WRITE_H_

#include <stdint.h>
#include "mliTar.h"
#include "mli_corsika_utils.h"
#include "mli_corsika_CorsikaPhotonBunch.h"

#define MLI_TARIO_CORSIKA_BUNCH_SIZE (sizeof(float) * 8)
#define MLI_TARIO_CORSIKA_HEADER_SIZE (sizeof(float) * 273)

struct mliTarIoWriter {
        struct mliTar tar;
        int event_number;
        int cherenkov_bunch_block_number;
        struct mliDynCorsikaPhotonBunch buffer;
};
struct mliTarIoWriter mliTarIoWriter_init(void);
int mliTarIoWriter_open(
        struct mliTarIoWriter *tio,
        const char *path,
        const uint64_t num_bunches_buffer);
int mliTarIoWriter_close(struct mliTarIoWriter *tio);
int mliTarIoWriter_add_runh(struct mliTarIoWriter *tio, const float *runh);
int mliTarIoWriter_add_evth(struct mliTarIoWriter *tio, const float *evth);
int mliTarIoWriter_add_cherenkov_bunch(
        struct mliTarIoWriter *tio,
        const struct mliCorsikaPhotonBunch bunch);
int mliTarIoWriter_add_cherenkov_bunch_raw(
        struct mliTarIoWriter *tio,
        const float *bunch_raw);
int mliTarIoWriter_finalize_cherenkov_bunch_block(struct mliTarIoWriter *tio);

struct mliTarIoReader {
        /* Current event-number */
        uint64_t event_number;

        /* Current cherenkov-block-number inside the current event */
        uint64_t cherenkov_bunch_block_number;

        /* Current bunch-number inside the current cherenkov-block */
        uint64_t block_at;
        uint64_t block_size;

        /* Underlying tape-archive */
        struct mliTar tar;

        /* Next file's tar-header in the underlying tape-archive */
        int has_tarh;
        struct mliTarHeader tarh;
};
struct mliTarIoReader mliTarIoReader_init(void);
int mliTarIoReader_open(struct mliTarIoReader *tio, const char *path);
int mliTarIoReader_close(struct mliTarIoReader *tio);
int mliTarIoReader_read_runh(struct mliTarIoReader *tio, float *runh);
int mliTarIoReader_read_evth(struct mliTarIoReader *tio, float *evth);
int mliTarIoReader_read_cherenkov_bunch(
        struct mliTarIoReader *tio,
        struct mliCorsikaPhotonBunch *bunch);
int mliTarIoReader_read_cherenkov_bunch_raw(
        struct mliTarIoReader *tio,
        float *bunch_raw);

int mliTarIoReader_tarh_is_valid_cherenkov_block(
        const struct mliTarIoReader *tio);
int mliTarIoReader_tarh_might_be_valid_cherenkov_block(
        const struct mliTarIoReader *tio);

void mliTarIo_testing_mark_bunch(
        struct mliCorsikaPhotonBunch *bunch,
        const uint64_t marker);
int mliTarIo_testing_bunch_has_mark(
        const struct mliCorsikaPhotonBunch bunch,
        const uint64_t marker);

#endif
