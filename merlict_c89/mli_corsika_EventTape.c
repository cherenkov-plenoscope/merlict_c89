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
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliDynCorsikaPhotonBunch_init();
        return tio;
}

int mliEventTapeWriter_close(struct mliEventTapeWriter *tio)
{
        if (tio->tar.stream) {
                if (tio->event_number) {
                        chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(
                                        tio),
                                "Can't finalize final event's "
                                "cherenkov-bunch-block");
                }
                chk_msg(mliTar_finalize(&tio->tar), "Can't finalize tar-file.");
                chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        }
        mliDynCorsikaPhotonBunch_free(&tio->buffer);
        (*tio) = mliEventTapeWriter_init();
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_open(
        struct mliEventTapeWriter *tio,
        const char *path,
        const uint64_t num_bunches_buffer)
{
        chk_msg(mliEventTapeWriter_close(tio),
                "Can't close and free previous tar-io-writer.");
        chk_msg(mliTar_open(&tio->tar, path, "w"), "Can't open tar.");
        chk_msg(mliDynCorsikaPhotonBunch_malloc(
                        &tio->buffer, num_bunches_buffer),
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
                        &tarh, path, MLI_CORSIKA_HEADER_SIZE),
                "Can't set tar-header for corsika-header.");

        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for corsika-header to tar.");

        chk_msg(mliTar_write_data(
                        &tio->tar,
                        corsika_header,
                        MLI_CORSIKA_HEADER_SIZE),
                "Can't write data of corsika-header to tar.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_runh(
        struct mliEventTapeWriter *tio,
        const float *runh)
{
        chk_msg(mliEventTapeWriter_write_corsika_header(
                        tio, "RUNH.float32", runh),
                "Can't write 'RUNH.float32' to tario.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};

        /* finalize previous event */

        if (tio->event_number) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize previous event's "
                        "cherenkov-bunch-block");
        }
        tio->event_number = (int)(MLI_ROUND(evth[1]));
        chk_msg(tio->event_number > 0, "Expected event_number > 0.");

        tio->cherenkov_bunch_block_number = 1;

        sprintf(path, "%09d/EVTH.float32", tio->event_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, evth),
                "Can't write 'EVTH.float32' to tario.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_flush_cherenkov_bunch_block(
        struct mliEventTapeWriter *tio)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        struct mliTarHeader tarh = mliTarHeader_init();
        float bunch_raw[8] = {0.0};
        uint64_t i = 0;

        sprintf(path,
                "%09d/cherenkov_bunches/%09d.Nx8_float32",
                tio->event_number,
                tio->cherenkov_bunch_block_number);

        chk_msg(mliTarHeader_set_normal_file(
                        &tarh,
                        path,
                        tio->buffer.size * MLI_CORSIKA_BUNCH_SIZE),
                "Can't set cherenkov-bunch-block's tar-header.");

        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for cherenkov-bunch-block to tar.");

        for (i = 0; i < tio->buffer.size; i++) {
                mliCorsikaPhotonBunch_to_raw(&tio->buffer.array[i], bunch_raw);
                chk_msg(mliTar_write_data(
                                &tio->tar,
                                bunch_raw,
                                MLI_CORSIKA_BUNCH_SIZE),
                        "Can't write cherenkov-bunch-block to tar-file.");
        }

        tio->buffer.size = 0;

        tio->cherenkov_bunch_block_number += 1;
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_cherenkov_bunch(
        struct mliEventTapeWriter *tio,
        const struct mliCorsikaPhotonBunch bunch)
{
        if (tio->buffer.size == tio->buffer.capacity) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
                chk_msg(tio->buffer.size == 0, "Expected buffer to be empty.");
        }
        tio->buffer.array[tio->buffer.size] = bunch;
        tio->buffer.size += 1;
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_cherenkov_bunch_raw(
        struct mliEventTapeWriter *tio,
        const float *bunch_raw)
{
        struct mliCorsikaPhotonBunch bunch;
        mliCorsikaPhotonBunch_set_from_raw(&bunch, bunch_raw);
        chk_msg(mliEventTapeWriter_write_cherenkov_bunch(tio, bunch),
                "Can't add raw-bunch to tar-io.");
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
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 0;
        tio.block_at = 0;
        tio.block_size = 0;
        return tio;
}

int mliEventTapeReader_close(struct mliEventTapeReader *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        }

        (*tio) = mliEventTapeReader_init();
        return 1;
error:
        return 0;
}

int mliEventTapeReader_open(struct mliEventTapeReader *tio, const char *path)
{
        chk_msg(mliEventTapeReader_close(tio),
                "Can't close and free previous tar-io-reader.");
        chk_msg(mliTar_open(&tio->tar, path, "r"), "Can't open tar.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh)
{
        chk_msg(tio->has_tarh, "Expected next tar-header.");
        chk_msg(strcmp(tio->tarh.name, "RUNH.float32") == 0,
                "Expected first file to be 'RUNH.float32.'");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE,
                "Expected RUNH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)runh, tio->tarh.size),
                "Can't read RUNH from tar.");
        chk_msg(runh[0] == mli_4chars_to_float("RUNH"),
                "Expected RUNH[0] == 'RUNH'");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_evth(struct mliEventTapeReader *tio, float *evth)
{
        uint64_t event_number_path, event_number_evth;
        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/EVTH.float32";

        if (!tio->has_tarh) {
                return 0;
        }
        chk_msg(mli_cstr_match_templeate(tio->tarh.name, match, 'd'),
                "Expected EVTH filename to match 'ddddddddd/EVTH.float32'.");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE,
                "Expected EVTH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)evth, tio->tarh.size),
                "Can't read EVTH from tar.");
        chk_msg(evth[0] == mli_4chars_to_float("EVTH"),
                "Expected EVTH[0] == 'EVTH'");
        chk_msg(mli_ncstr_to_uint64(&event_number_path, tio->tarh.name, 10, 9),
                "Can't parse event-number from path.");
        event_number_evth = (uint64_t)evth[MLI_CORSIKA_EVTH_EVENT_NUMBER];
        chk_msg(event_number_evth == event_number_path,
                "Expected same event-number in path and EVTH.");
        tio->event_number = event_number_evth;
        tio->cherenkov_bunch_block_number = 1;

        /* now there must follow a cherenkov-bunch-block */
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        chk_msg(tio->has_tarh, "Expected cherenkov-bunch-block after EVTH.");
        chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                "Cherenkov-bunch-block's tar-header doesn't match.");

        chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE == 0,
                "Expected cherenkov-bunch-block-size "
                "to be multiple of bunch-size.");
        tio->block_size = tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE;
        tio->block_at = 0;
        return 1;
error:
        return 0;
}

int mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        char match[MLI_TAR_NAME_LENGTH] =
                "ddddddddd/cherenkov_bunches/ddddddddd.Nx8_float32";
        return mli_cstr_match_templeate(tio->tarh.name, match, 'd');
}

int mliEventTapeReader_tarh_is_valid_cherenkov_block(
        const struct mliEventTapeReader *tio)
{
        uint64_t event_number_path, block_number_path;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");

        chk_msg(mliEventTapeReader_tarh_might_be_valid_cherenkov_block(tio),
                "Expected cherenkov-bunch-block-name to be valid.");

        chk_msg(mli_ncstr_to_uint64(
                        &event_number_path, &tio->tarh.name[0], 10, 9),
                "Can't parse event-number from path.");

        chk_msg(event_number_path == tio->event_number,
                "Expected same event-number in cherenkov-block-path and EVTH.");

        chk_msg(mli_ncstr_to_uint64(
                        &block_number_path, &tio->tarh.name[28], 10, 9),
                "Can't parse cherenkov-block-number from path.");

        chk_msg(block_number_path == tio->cherenkov_bunch_block_number,
                "Expected different cherenkov-bunch-block-number in "
                "cherenkov-block-path.");
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_cherenkov_bunch(
        struct mliEventTapeReader *tio,
        struct mliCorsikaPhotonBunch *bunch)
{
        float raw[8];
        int rc = mliEventTapeReader_read_cherenkov_bunch_raw(tio, raw);
        if (rc == 1) {
                mliCorsikaPhotonBunch_set_from_raw(bunch, raw);
                return 1;
        } else {
                return 0;
        }
}

int mliEventTapeReader_read_cherenkov_bunch_raw(
        struct mliEventTapeReader *tio,
        float *bunch_raw)
{
        if (tio->block_at == tio->block_size) {
                tio->cherenkov_bunch_block_number += 1;
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);

                if (!tio->has_tarh) {
                        return 0;
                }
                if (!mliEventTapeReader_tarh_might_be_valid_cherenkov_block(
                            tio)) {
                        return 0;
                }
                chk_msg(mliEventTapeReader_tarh_is_valid_cherenkov_block(tio),
                        "Cherenkov-bunch-block's tar-header doesn't match.");

                chk_msg(tio->tarh.size % MLI_CORSIKA_BUNCH_SIZE == 0,
                        "Expected cherenkov-bunch-block-size "
                        "to be multiple of bunch-size.");
                tio->block_size =
                        tio->tarh.size / MLI_CORSIKA_BUNCH_SIZE;
                tio->block_at = 0;
        }

        if (tio->block_size == 0) {
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
                return 0;
        }

        chk_msg(mliTar_read_data(
                        &tio->tar,
                        (void *)(bunch_raw),
                        MLI_CORSIKA_BUNCH_SIZE),
                "Failed to read cherenkov_bunch.");

        tio->block_at += 1;

        return 1;
error:
        return 0;
}
