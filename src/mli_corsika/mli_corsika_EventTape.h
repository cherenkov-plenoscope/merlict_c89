/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_EVENTTAPE_H_
#define MLI_CORSIKA_EVENTTAPE_H_

#include <stdint.h>
#include "../mli/mliTar.h"
#include "../mli/mliDynFloat.h"
#include "mli_corsika_utils.h"

#define MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR 2
#define MLI_CORSIKA_EVENTTAPE_VERSION_MINOR 1
#define MLI_CORSIKA_EVENTTAPE_VERSION_PATCH 2

struct mliEventTapeWriter {
        struct mliTar tar;
        int flush_tar_stream_after_each_file;
        int run_number;
        int event_number;
        int cherenkov_bunch_block_number;
        struct mliDynFloat buffer;
};
struct mliEventTapeWriter mliEventTapeWriter_init(void);

int mliEventTapeWriter_begin(
        struct mliEventTapeWriter *tio,
        FILE *stream,
        const uint64_t num_bunches_buffer);
int mliEventTapeWriter_finalize(struct mliEventTapeWriter *tio);

int mliEventTapeWriter_write_runh(
        struct mliEventTapeWriter *tio,
        const float *runh);
int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth);
int mliEventTapeWriter_write_cherenkov_bunch(
        struct mliEventTapeWriter *tio,
        const float *bunch);
int mliEventTapeWriter_flush_cherenkov_bunch_block(
        struct mliEventTapeWriter *tio);

struct mliEventTapeReader {
        uint64_t run_number;

        /* Current event-number */
        uint64_t event_number;

        /* Current cherenkov-block-number inside the current event */
        uint64_t cherenkov_bunch_block_number;

        /* Current bunch-number inside the current cherenkov-block */
        uint64_t block_at;
        uint64_t block_size;
        int has_still_bunches_in_event;

        /* Underlying tape-archive */
        struct mliTar tar;

        /* Next file's tar-header in the underlying tape-archive */
        int has_tarh;
        struct mliTarHeader tarh;
};
struct mliEventTapeReader mliEventTapeReader_init(void);

int mliEventTapeReader_begin(struct mliEventTapeReader *tio, FILE *stream);
int mliEventTapeReader_finalize(struct mliEventTapeReader *tio);

int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh);
int mliEventTapeReader_read_evth(struct mliEventTapeReader *tio, float *evth);
int mliEventTapeReader_read_cherenkov_bunch(
        struct mliEventTapeReader *tio,
        float *bunch);
int mliEventTapeReader_tarh_is_valid_cherenkov_block(
        const struct mliEventTapeReader *tio);
int mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
        const struct mliEventTapeReader *tio);
int mliEventTapeReader_read_readme_until_runh(struct mliEventTapeReader *tio);

#endif
