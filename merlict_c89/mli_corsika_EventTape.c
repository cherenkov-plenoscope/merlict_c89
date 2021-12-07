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

int mliEventTapeWriter_close(struct mliEventTapeWriter *tio)
{
        if (tio->tar.stream) {
                chk_msg(mliTar_finalize(&tio->tar), "Can't finalize tar-file.");
                chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        }
        mliDynFloat_free(&tio->buffer);
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
        chk_msg(mliDynFloat_malloc(&tio->buffer, 8 * num_bunches_buffer),
                "Can't malloc cherenkov-bunch-buffer.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_readme(
    struct mliEventTapeWriter *tio,
    const char *path)
{
        struct mliTarHeader tarh = mliTarHeader_init();
        char vers[1024];
        uint64_t p = 0;
        p += sprintf(vers + p, "MLI_VERSION_MAYOR %d\n", MLI_VERSION_MAYOR);
        p += sprintf(vers + p, "MLI_VERSION_MINOR %d\n", MLI_VERSION_MINOR);
        p += sprintf(vers + p, "MLI_VERSION_PATCH %d\n", MLI_VERSION_PATCH);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_VERSION_MAYOR %d\n",
                        MLI_CORSIKA_VERSION_MAYOR);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_VERSION_MINOR %d\n",
                        MLI_CORSIKA_VERSION_MINOR);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_VERSION_PATCH %d\n",
                        MLI_CORSIKA_VERSION_PATCH);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR %d\n",
                        MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_EVENTTAPE_VERSION_MINOR %d\n",
                        MLI_CORSIKA_EVENTTAPE_VERSION_MINOR);
        p +=
                sprintf(vers + p,
                        "MLI_CORSIKA_EVENTTAPE_VERSION_PATCH %d\n",
                        MLI_CORSIKA_EVENTTAPE_VERSION_PATCH);
        chk_msg(p < sizeof(vers), "Info string is too long.");

        chk_msg(mliTarHeader_set_normal_file(&tarh, path, p),
                "Can't set tar-header for 'version.txt'.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for 'version.txt' to tar.");
        chk_msg(mliTar_write_data(&tio->tar, vers, p),
                "Can't write data of 'version.txt' to tar.");

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
                        &tio->tar, corsika_header, MLI_CORSIKA_HEADER_SIZE_BYTES),
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

        sprintf(path, "%09d/version.txt", tio->run_number);
        chk_msg(mliEventTapeWriter_write_readme(tio, path),
                "Can't write info.");

        sprintf(path, "%09d/RUNH.float32", tio->run_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, runh),
                "Can't write 'RUNH.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_rune(
        struct mliEventTapeWriter *tio,
        const float *rune)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        chk_msg(tio->run_number > 0, "Expected RUNH before RUNE.");
        sprintf(path, "%09d/RUNE.float32", tio->run_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, rune),
                "Can't write 'RUNE.float32' to event-tape.");
        return 1;
error:
        return 0;
}

int mliEventTapeWriter_write_evth(
        struct mliEventTapeWriter *tio,
        const float *evth)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        int run_number_in_evth = (int)(
                MLI_ROUND(evth[MLI_CORSIKA_EVTH_RUN_NUMBER]));

        chk_msg(tio->run_number != 0, "Expected RUNH before EVTH.");
        chk_msg(tio->run_number == run_number_in_evth,
                "Expected run_number in EVTH "
                "to match run_number in last RUNH.");

        tio->event_number = (int)(
                MLI_ROUND(evth[MLI_CORSIKA_EVTH_EVENT_NUMBER]));
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

int mliEventTapeWriter_write_evte(
        struct mliEventTapeWriter *tio,
        const float *evte)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        chk_msg(tio->run_number > 0, "Expected RUNH and EVTH before EVTE.");
        chk_msg(tio->event_number > 0, "Expected EVTH before EVTE.");

        /* finalize previous event */
        if (tio->event_number) {
                chk_msg(mliEventTapeWriter_flush_cherenkov_bunch_block(tio),
                        "Can't finalize previous event's "
                        "cherenkov-bunch-block");
        }

        sprintf(path,
                "%09d/%09d/EVTE.float32",
                tio->run_number,
                tio->event_number);
        chk_msg(mliEventTapeWriter_write_corsika_header(tio, path, evte),
                "Can't write 'EVTE.float32' to event-tape.");
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
                chk_msg(tio->buffer.size == 0, "Expected buffer to be empty.");
        }
        chk_msg(tio->buffer.size + 8 <= tio->buffer.capacity,
                "Not enough capacity in buffer to push back Cherenkov-bunch.");

        for (i = 0; i < 8; i ++) {
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

        chk_msg(mliEventTapeReader_read_readme_until_runh(tio),
                "Can't read info.");

        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_readme_until_runh(struct mliEventTapeReader *tio)
{
        int i = 0;
        while (1) {
                chk_msg(i < 128, "Expected < 128 files before 'RUNH.float32'.");
                if (tio->has_tarh) {
                        if (mli_cstr_match_templeate(tio->tarh.name, "ddddddddd/RUNH.float32", 'd')) {
                                break;
                        } else {
                                /* read readme's payload */
                                uint64_t c;
                                char payload;
                                for (c = 0; c < tio->tarh.size; c++) {
                                        chk_msg(mliTar_read_data(
                                                        &tio->tar, &payload, 1),
                                                "Can't read readme's data.");
                                }
                                tio->has_tarh = mliTar_read_header(
                                        &tio->tar, &tio->tarh);
                                i += 1;
                        }
                } else {
                        break;
                }
        }
        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_runh(struct mliEventTapeReader *tio, float *runh)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t run_number_from_evth = 0;

        chk_msg(tio->has_tarh, "Expected next tar-header.");
        chk_msg(mli_cstr_match_templeate(
                    tio->tarh.name,
                    "ddddddddd/RUNH.float32",
                    'd'),
                "Expected file to be 'ddddddddd/RUNH.float32.'");
        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected RUNH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)runh, tio->tarh.size),
                "Can't read RUNH from tar.");
        chk_msg(runh[0] == mli_chars_to_float("RUNH"),
                "Expected RUNH[0] == 'RUNH'");

        chk_msg(mli_cstr_nto_uint64(
                &tio->run_number,
                &tio->tarh.name[0],
                BASE,
                NUM_DIGITS),
                "Can't read run_number from RUNH's path.");

        run_number_from_evth = (uint64_t)(
                MLI_ROUND(runh[MLI_CORSIKA_RUNH_RUN_NUMBER]));
        chk_msg(tio->run_number == run_number_from_evth,
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
        uint64_t event_number_path;
        uint64_t event_number_evth;
        uint64_t run_number_path;
        uint64_t run_number_evth;

        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/ddddddddd/EVTH.float32";

        if (!tio->has_tarh) {
                return 0;
        }
        if (!mli_cstr_match_templeate(tio->tarh.name, match, 'd')) {
                return 0;
        }

        chk_msg(mli_cstr_nto_uint64(
                        &event_number_path,
                        &tio->tarh.name[10],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(mli_cstr_nto_uint64(
                        &run_number_path,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");

        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected EVTH to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)evth, tio->tarh.size),
                "Can't read EVTH from tar.");
        chk_msg(evth[0] == mli_chars_to_float("EVTH"),
                "Expected EVTH[0] == 'EVTH'");

        event_number_evth = (uint64_t)evth[MLI_CORSIKA_EVTH_EVENT_NUMBER];
        run_number_evth = (uint64_t)evth[MLI_CORSIKA_EVTH_RUN_NUMBER];

        chk_msg(event_number_evth == event_number_path,
                "Expected paths' event-number to match event-number in EVTH.");
        chk_msg(run_number_evth == run_number_path,
                "Expected paths' run-number to match run-number in EVTH.");

        tio->event_number = event_number_evth;
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

int mliEventTapeReader_read_evte(struct mliEventTapeReader *tio, float *evte)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t event_number_path;
        uint64_t run_number_path;

        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/ddddddddd/EVTE.float32";

        chk_msg(mli_cstr_match_templeate(tio->tarh.name, match, 'd'),
                "Expected EVTE filename to match "
                "'ddddddddd/ddddddddd/EVTE.float32'.");

        chk_msg(mli_cstr_nto_uint64(
                        &event_number_path,
                        &tio->tarh.name[10],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(mli_cstr_nto_uint64(
                        &run_number_path,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");

        chk_msg(tio->event_number == event_number_path,
                "Expected paths' event-number to match event-number in Reader.");
        chk_msg(tio->run_number == run_number_path,
                "Expected paths' run-number to match run-number in Reader.");

        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected EVTE to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)evte, tio->tarh.size),
                "Can't read EVTE from tar.");

        chk_msg(evte[0] == mli_chars_to_float("EVTE"),
                "Expected EVTE[0] == 'EVTE'");

        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);

        return 1;
error:
        return 0;
}

int mliEventTapeReader_read_rune(struct mliEventTapeReader *tio, float *rune)
{
        const uint64_t NUM_DIGITS = 9;
        const uint64_t BASE = 10;
        uint64_t run_number_path;

        char match[MLI_TAR_NAME_LENGTH] = "ddddddddd/RUNE.float32";

        chk_msg(mli_cstr_match_templeate(tio->tarh.name, match, 'd'),
                "Expected RUNE filename to match "
                "'ddddddddd/RUNE.float32'.");
        chk_msg(mli_cstr_nto_uint64(
                        &run_number_path,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(tio->run_number == run_number_path,
                "Expected paths' run-number to match run-number in Reader.");

        chk_msg(tio->tarh.size == MLI_CORSIKA_HEADER_SIZE_BYTES,
                "Expected RUNE to have size 273*sizeof(float)");
        chk_msg(mliTar_read_data(&tio->tar, (void *)rune, tio->tarh.size),
                "Can't read RUNE from tar.");

        chk_msg(rune[0] == mli_chars_to_float("RUNE"),
                "Expected RUNE[0] == 'RUNE'");

        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);

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
        uint64_t run_number_path;
        uint64_t event_number_path;
        uint64_t block_number_path;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");

        chk_msg(mliEventTapeReader_tarh_might_be_valid_cherenkov_block(tio),
                "Expected cherenkov-bunch-block-name to be valid.");

        chk_msg(mli_cstr_nto_uint64(
                        &run_number_path,
                        &tio->tarh.name[0],
                        BASE,
                        NUM_DIGITS),
                "Can't parse run-number from path.");
        chk_msg(run_number_path == tio->run_number,
                "Expected consistent run-number in cherenkov-block-path.");

        chk_msg(mli_cstr_nto_uint64(
                        &event_number_path,
                        &tio->tarh.name[9 + 1],
                        BASE,
                        NUM_DIGITS),
                "Can't parse event-number from path.");
        chk_msg(event_number_path == tio->event_number,
                "Expected same event-number in cherenkov-block-path and EVTH.");

        chk_msg(mli_cstr_nto_uint64(
                        &block_number_path,
                        &tio->tarh.name[9 + 1 + 9 + 1],
                        BASE,
                        NUM_DIGITS),
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
                        &tio->tar, (void *)(bunch), MLI_CORSIKA_BUNCH_SIZE_BYTES),
                "Failed to read cherenkov_bunch.");

        tio->block_at += 1;

        return 1;
error:
        return 0;
}
