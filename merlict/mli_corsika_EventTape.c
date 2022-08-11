/* Copyright 2020 Sebastian A. Mueller */
#include "mli_corsika_EventTape.h"
#include "mli_corsika_utils.h"
#include "mliTar.h"

/* writer */
/* ====== */
struct mliEventTapeWriter mliEventTapeWriter_init(void)
{
        struct mliEventTapeWriter tio;
        tio.tar = mliTar_init();
        tio.run_number = 0;
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliDynFloat_init();
        return tio;
}

int mliEventTapeWriter_finalize(struct mliEventTapeWriter *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
                chk_msg(mliTar_write_finalize(&tio->tar),
                        "Can't finalize tar-file.");
        }
        mliDynFloat_free(&tio->buffer);
        (*tio) = mliEventTapeWriter_init();
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_begin(
        struct mliEventTapeWriter *tio,
        FILE *stream,
        const uint64_t num_bunches_buffer)
{
        chk_msg(mliEventTapeWriter_finalize(tio),
                "Can't close and free previous tar-io-writer.");
        chk_msg(mliTar_write_begin(&tio->tar, stream), "Can't begin tar.");
        chk_msg(mliDynFloat_malloc(&tio->buffer, 8 * num_bunches_buffer),
                "Can't malloc cherenkov-bunch-buffer.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_corsika_header(
        struct mliEventTapeWriter *tio,
        const char *path,
        const float *corsika_header)
{
        struct mliTarHeader tarh = mliTarHeader_init();
        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, path, MLI_CORSIKA_HEADER_SIZE_BYTES),
                "Can't set tar-header for corsika-header.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for corsika-header to tar.");
        chk_msg(mliTar_write_data(
                        &tio->tar,
                        corsika_header,
                        MLI_CORSIKA_HEADER_SIZE_BYTES),
                "Can't write data of corsika-header to tar.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_runh(
        struct mliEventTapeWriter *tio,
        const float *runh)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        tio->run_number = (int)(MLI_ROUND(runh[1]));
        chk_msg(tio->run_number >= 1, "Expected run_number >= 1.");
        sprintf(path, "%09d/RUNH.float32", tio->run_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, runh),
                "Can't write 'RUNH.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        int evth_run_number =
                (int)(MLI_ROUND(evth[MLI_CORSIKA_EVTH_RUN_NUMBER]));

        if (tio->event_number > 0) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
        }
        chk_msg(tio->run_number != 0, "Expected RUNH before EVTH.");
        chk_msg(tio->run_number == evth_run_number,
                "Expected run_number in EVTH "
                "to match run_number in last RUNH.");
        tio->event_number =
                (int)(MLI_ROUND(evth[MLI_CORSIKA_EVTH_EVENT_NUMBER]));
        chk_msg(tio->event_number > 0, "Expected event_number > 0.");
        tio->cherenkov_bunch_block_number = 1;
        sprintf(path,
                "%09d/%09d/EVTH.float32",
                tio->run_number,
                tio->event_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, evth),
                "Can't write 'EVTH.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_flush_cherenkov_bunch_block(
        struct mliEventTapeWriter *tio)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        struct mliTarHeader tarh = mliTarHeader_init();
        sprintf(path,
                "%09d/%09d/%09d.cer.x8.float32",
                tio->run_number,
                tio->event_number,
                tio->cherenkov_bunch_block_number);
        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, path, tio->buffer.size * sizeof(float)),
                "Can't set cherenkov-bunch-block's tar-header.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for cherenkov-bunch-block to tar.");
        chk_msg(mliTar_write_data(
                        &tio->tar,
                        tio->buffer.array,
                        tio->buffer.size * sizeof(float)),
                "Can't write cherenkov-bunch-block to tar-file.");
        tio->buffer.size = 0;
        tio->cherenkov_bunch_block_number += 1;
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_cherenkov_bunch(
        struct mliEventTapeWriter *tio,
        const float *bunch)
{
        uint64_t i;
        if (tio->buffer.size == tio->buffer.capacity) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
        }
        for (i = 0; i < 8; i++) {
                tio->buffer.array[tio->buffer.size] = bunch[i];
                tio->buffer.size += 1;
        }
        return 1;
error:
        return 0;
}

/* reader */
/* ====== */

struct mliEventTapeReader mliEventTapeReader_init(void)
{
        struct mliEventTapeReader tio;
        tio.tar = mliTar_init();
        tio.tarh = mliTarHeader_init();
        tio.run_number = 0;
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 0;
        tio.block_at = 0;
        tio.block_size = 0;
        tio.has_still_bunches_in_event = 0;
        return tio;
}

int mliEventTapeReader_finalize(struct mliEventTapeReader *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliTar_read_finalize(&tio->tar),
                        "Can't finalize reading tar.");
        }
        (*tio) = mliEventTapeReader_init();
        return 1;
error:
        return 0;
}

int mliEventTapeReader_begin(struct mliEventTapeReader *tio, FILE *stream)
{
        chk_msg(mliEventTapeReader_finalize(tio),
                "Can't close and free previous tar-io-reader.");
        chk_msg(mliTar_read_begin(&tio->tar, stream), "Can't begin tar.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t runh_run_number = 0;
        chk_msg(tio->has_tarh, "Expected next tar-header.");
        chk_msg(mli_cstr_match_templeate(
                        tio->tarh.name, "ddddddddd/RUNH.float32", 'd'),
                "Expected file to be 'ddddddddd/RUNH.float32.'");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected RUNH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)runh, tio->tarh.size),
                "Can't read RUNH from tar.");
        chk_msg(runh[0] == mli_chars_to_float("RUNH"),
                "Expected RUNH[0] == 'RUNH'");
        chk_msg(mli_cstr_nto_uint64(
                        &tio->run_number, &tio->tarh.name[0], BASE, NUM_DIGITS),
                "Can't read run_number from RUNH's path.");
        runh_run_number =
                (uint64_t)(MLI_ROUND(runh[MLI_CORSIKA_RUNH_RUN_NUMBER]));
        chk_msg(tio->run_number == runh_run_number,
                "Expected run_number in RUNH's path "
                "to match run_number in RUNH.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_evth(struct mliEventTapeReader *tio, float *evth)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t path_event_number;
        uint64_t evth_event_number;
        uint64_t path_run_number;
        uint64_t evth_run_number;
        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/ddddddddd/EVTH.float32";

        if (!tio->has_tarh) {
                return 0;
        }
        if (!mli_cstr_match_templeate(tio->tarh.name, match, 'd')) {
                return 0;
        }
        chk_msg(mli_cstr_nto_uint64(
                        &path_event_number,
                        &tio->tarh.name[10],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(mli_cstr_nto_uint64(
                        &path_run_number, &tio->tarh.name[0], BASE, NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected EVTH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)evth, tio->tarh.size),
                "Can't read EVTH from tar.");
        chk_msg(evth[0] == mli_chars_to_float("EVTH"),
                "Expected EVTH[0] == 'EVTH'");

        evth_event_number = (uint64_t)evth[MLI_CORSIKA_EVTH_EVENT_NUMBER];
        evth_run_number = (uint64_t)evth[MLI_CORSIKA_EVTH_RUN_NUMBER];

        chk_msg(evth_event_number == path_event_number,
                "Expected paths' event-number to match event-number in EVTH.");
        chk_msg(evth_run_number == path_run_number,
                "Expected paths' run-number to match run-number in EVTH.");

        tio->event_number = evth_event_number;
        tio->cherenkov_bunch_block_number = 1;

        /* now there must follow a cherenkov-bunch-block */
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);

        chk_msg(tio->has_tarh, "Expected cherenkov-bunch-block after EVTH.");
        chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                "Cherenkov-bunch-block's tar-header doesn't match.");

        chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE_BYTES == 0,
                "Expected cherenkov-bunch-block-size "
                "to be multiple of bunch-size.");
        tio->block_size = tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE_BYTES;
        tio->block_at = 0;
        tio->has_still_bunches_in_event = 1;

        return 1;
error:
        return 0;
}

int mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        char match[MLI_TAR_NAME_LENGTH] =
                "ddddddddd/ddddddddd/ddddddddd.cer.x8.float32";
        return mli_cstr_match_templeate(tio->tarh.name, match, 'd');
}

int mliEventTapeReader_tarh_is_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t path_run_number;
        uint64_t path_event_number;
        uint64_t path_block_number;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");

        chk_msg(mliEventTapeReader_tarh_might_be_valid_cherenkov_block(tio),
                "Expected cherenkov-bunch-block-name to be valid.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_run_number, &tio->tarh.name[0], BASE, NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(path_run_number == tio->run_number,
                "Expected consistent run-number in cherenkov-block-path.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_event_number,
                        &tio->tarh.name[9 + 1],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(path_event_number == tio->event_number,
                "Expected same event-number in cherenkov-block-path and EVTH.");

        chk_msg(mli_cstr_nto_uint64(
                        &path_block_number,
                        &tio->tarh.name[9 + 1 + 9 + 1],
                        BASE,
                        NUM_DIGITS),
                "Can't parse cherenkov-block-number from path.");
        chk_msg(path_block_number == tio->cherenkov_bunch_block_number,
                "Expected different cherenkov-bunch-block-number in "
                "cherenkov-block-path.");
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_cherenkov_bunch(
        struct mliEventTapeReader *tio,
        float *bunch)
{
        if (tio->has_still_bunches_in_event == 0) {
                return 0;
        }
        if (tio->block_at == tio->block_size) {
                tio->cherenkov_bunch_block_number += 1;
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
                if (!tio->has_tarh) {
                        tio->has_still_bunches_in_event = 0;
                        return 0;
                }
                if (!mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
                            tio)) {
                        tio->has_still_bunches_in_event = 0;
                        return 0;
                }
                chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                        "Cherenkov-bunch-block's tar-header doesn't match.");
                chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE_BYTES == 0,
                        "Expected cherenkov-bunch-block-size "
                        "to be multiple of bunch-size.");
                tio->block_size = tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE_BYTES;
                tio->block_at = 0;
        }
        chk_msg(mliTar_read_data(
                        &tio->tar,
                        (void *)(bunch),
                        MLI_CORSIKA_BUNCH_SIZE_BYTES),
                "Failed to read cherenkov_bunch.");

        tio->block_at += 1;
        return 1;
error:
        return 0;
}
