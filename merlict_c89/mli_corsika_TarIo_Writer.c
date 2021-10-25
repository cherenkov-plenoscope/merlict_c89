/* Copyright 2020 Sebastian A. Mueller */
#include "mli_corsika_TarIo_Writer.h"
#include "mli_corsika_utils.h"
#include "mliTar.h"

/* buffer */
struct mliTarIoCherenkovBunchBuffer mliTarIoCherenkovBunchBuffer_init(void)
{
        struct mliTarIoCherenkovBunchBuffer buffer;
        buffer.capacity = 0;
        buffer.size = 0;
        buffer.bunches = NULL;
        return buffer;
}
void mliTarIoCherenkovBunchBuffer_free(struct mliTarIoCherenkovBunchBuffer *b)
{
        free(b->bunches);
        (*b) = mliTarIoCherenkovBunchBuffer_init();
}

int mliTarIoCherenkovBunchBuffer_malloc(
        struct mliTarIoCherenkovBunchBuffer *b,
        const uint64_t capacity)
{
        mliTarIoCherenkovBunchBuffer_free(b);
        chk_msg(capacity > 0, "Expected buffer size > 0");
        b->capacity = capacity;
        chk_malloc(
                b->bunches, float, b->capacity *MLI_TARIO_CORSIKA_BUNCH_SIZE);
        return 1;
error:
        return 0;
}

/* file structure */
struct mliTarIoWriter mliTarIoWriter_init(void)
{
        struct mliTarIoWriter tio;
        tio.tar = mliTar_init();
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliTarIoCherenkovBunchBuffer_init();
        return tio;
}

int mliTarIoWriter_open(
        struct mliTarIoWriter *tio,
        const char *path,
        const int capacity)
{
        chk_msg(mliTar_open(&tio->tar, path, "w"), "Can't open tar.");
        chk_msg(mliTarIoCherenkovBunchBuffer_malloc(&tio->buffer, capacity),
                "Can't malloc cherenkov-bunch-buffer.");
        return 1;
error:
        return 0;
}

int mliTarIoWriter_add_corsika_header(
        struct mliTarIoWriter *tio,
        const char *path,
        const float *corsika_header)
{
        struct mliTarHeader tarh = mliTarHeader_init();
        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, path, MLI_TARIO_CORSIKA_HEADER_SIZE),
                "Can't set tar-header for corsika-header.");

        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for corsika-header to tar.");

        chk_msg(mliTar_write_data(
                        &tio->tar,
                        corsika_header,
                        MLI_TARIO_CORSIKA_HEADER_SIZE),
                "Can't write data of corsika-header to tar.");
        return 1;
error:
        return 0;
}

int mliTarIoWriter_add_runh(struct mliTarIoWriter *tio, const float *runh)
{
        chk_msg(mliTarIoWriter_add_corsika_header(tio, "RUNH.float32", runh),
                "Can't write 'RUNH.float32' to tario.");
        return 1;
error:
        return 0;
}

int mliTarIoWriter_add_evth(struct mliTarIoWriter *tio, const float *evth)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};

        /* finalize previous event */

        if (tio->event_number) {
                chk_msg(mliTarIoWriter_finalize_cherenkov_bunch_block(tio),
                        "Can't finalize previous event's "
                        "cherenkov-bunch-block");
        }
        tio->event_number = (int)(MLI_ROUND(evth[1]));
        chk_msg(tio->event_number > 0, "Expected event_number > 0.");

        tio->cherenkov_bunch_block_number = 1;

        sprintf(path, "%09d/EVTH.float32", tio->event_number);
        chk_msg(mliTarIoWriter_add_corsika_header(tio, path, evth),
                "Can't write 'EVTH.float32' to tario.");
        return 1;
error:
        return 0;
}

int mliTarIoWriter_finalize_cherenkov_bunch_block(struct mliTarIoWriter *tio)
{
        char path[MLI_TAR_NAME_LENGTH] = {'\0'};
        struct mliTarHeader tarh = mliTarHeader_init();

        sprintf(path,
                "%09d/%09d.cherenkov_bunches.Nx8_float32",
                tio->event_number,
                tio->cherenkov_bunch_block_number);

        chk_msg(mliTarHeader_set_normal_file(
                        &tarh,
                        path,
                        tio->buffer.size * MLI_TARIO_CORSIKA_BUNCH_SIZE),
                "Can't set cherenkov-bunch-block's tar-header.");

        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for cherenkov-bunch-block to tar.");

        chk_msg(mliTar_write_data(
                        &tio->tar,
                        tio->buffer.bunches,
                        tio->buffer.size * MLI_TARIO_CORSIKA_BUNCH_SIZE),
                "Can't write cherenkov-bunch-block to tar-file.");
        tio->buffer.size = 0;

        tio->cherenkov_bunch_block_number += 1;
        return 1;
error:
        return 0;
}

int mliTarIoWriter_add_cherenkov_bunch(
        struct mliTarIoWriter *tio,
        const float bunch[8])
{
        uint64_t i;
        if (tio->buffer.size == tio->buffer.capacity) {
                chk_msg(mliTarIoWriter_finalize_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
                chk_msg(tio->buffer.size == 0, "Expected buffer to be empty.");
        }
        for (i = 0; i < 8; i++) {
                tio->buffer.bunches[tio->buffer.size * 8 + i] = bunch[i];
        }
        tio->buffer.size += 1;
        return 1;
error:
        return 0;
}

int mliTarIoWriter_close(struct mliTarIoWriter *tio)
{
        if (tio->event_number) {
                chk_msg(mliTarIoWriter_finalize_cherenkov_bunch_block(tio),
                        "Can't finalize final event's cherenkov-bunch-block");
        }
        chk_msg(mliTar_finalize(&tio->tar), "Can't finalize tar-file.");
        chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        mliTarIoCherenkovBunchBuffer_free(&tio->buffer);
        return 1;
error:
        return 0;
}

struct mliTarIoReader mliTarIoReader_init(void)
{
        struct mliTarIoReader tio;
        tio.tar = mliTar_init();
        tio.tarh = mliTarHeader_init();
        tio.event_number = 0;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliTarIoCherenkovBunchBuffer_init();
        tio.buffer_at = 0;
        tio.bunch_number = 0;
        return tio;
}

int mliTarIoReader_open(struct mliTarIoReader *tio, const char *path)
{
        chk_msg(mliTar_open(&tio->tar, path, "r"), "Can't open tar.");
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliTarIoReader_read_runh(struct mliTarIoReader *tio, float *runh)
{
        chk_msg(tio->has_tarh, "Expected next tar-header.");
        chk_msg(strcmp(tio->tarh.name, "RUNH.float32") == 0,
                "Expected first file to be 'RUNH.float32.'");
        chk_msg(tio->tarh.size == MLI_TARIO_CORSIKA_HEADER_SIZE,
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

int mliTarIoReader_read_evth(struct mliTarIoReader *tio, float *evth)
{
        uint64_t event_number_path, event_number_evth;

        if (!tio->has_tarh) {
                return 0;
        }
        chk_msg(strlen(tio->tarh.name) == 9 + 1 + strlen("EVTH.float32"),
                "Expected path length.");
        chk_msg(strcmp(&tio->tarh.name[10], "EVTH.float32") == 0,
                "Expected path suffix 'EVTH.float32.'");
        chk_msg(tio->tarh.size == MLI_TARIO_CORSIKA_HEADER_SIZE,
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
        tio->bunch_number = 0;
        tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
        return 1;
error:
        return 0;
}

int mliTarIoReader_tarh_might_be_valid_cherenkov_block(
        const struct mliTarIoReader *tio)
{
        if (strcmp(&tio->tarh.name[9 + 1 + 9],
                   ".cherenkov_bunches.Nx8_float32") == 0) {
                return 1;
        } else {
                return 0;
        }
}

int mliTarIoReader_tarh_is_valid_cherenkov_block(
        const struct mliTarIoReader *tio)
{
        uint64_t event_number_path, block_number_path;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");
        chk_msg(strlen(tio->tarh.name) ==
                        9 + 1 + 9 + strlen(".cherenkov_bunches.Nx8_float32"),
                "Expected path length.");
        chk_msg(strcmp(&tio->tarh.name[9 + 1 + 9],
                       ".cherenkov_bunches.Nx8_float32") == 0,
                "Expected path suffix '.cherenkov_bunches.Nx8_float32.'");

        chk_msg(mli_ncstr_to_uint64(
                        &event_number_path, &tio->tarh.name[0], 10, 9),
                "Can't parse event-number from path.");

        chk_msg(event_number_path == tio->event_number,
                "Expected same event-number in cherenkov-block-path and EVTH.");

        chk_msg(mli_ncstr_to_uint64(
                        &block_number_path, &tio->tarh.name[10], 10, 9),
                "Can't parse cherenkov-block-number from path.");

        chk_msg(block_number_path == tio->cherenkov_bunch_block_number,
                "Expected different cherenkov-bunch-block-number in "
                "cherenkov-block-path.");
        return 1;
error:
        return 0;
}

int mliTarIoReader_malloc_buffer(struct mliTarIoReader *tio)
{
        uint64_t capacity;
        chk_msg(tio->has_tarh, "Expected a next tar-header.");
        chk_msg(tio->tarh.size > 0, "Expected buffer-size > 0.");
        chk_msg(tio->tarh.size % MLI_TARIO_CORSIKA_BUNCH_SIZE == 0,
                "Expected buffer-size to be multiple of bunch-size.");
        capacity = tio->tarh.size / MLI_TARIO_CORSIKA_BUNCH_SIZE;
        chk_msg(mliTarIoCherenkovBunchBuffer_malloc(&tio->buffer, capacity),
                "Can't malloc cherenkov-bunch-buffer.");
        return 1;
error:
        return 0;
}

int mliTarIoReader_read_buffer(struct mliTarIoReader *tio)
{
        chk_msg(tio->has_tarh, "Expected a next tar-header.");
        chk_msg(tio->tarh.size % MLI_TARIO_CORSIKA_BUNCH_SIZE == 0,
                "Expected buffer-size to be multiple of bunch-size.");
        tio->buffer.size = tio->tarh.size / MLI_TARIO_CORSIKA_BUNCH_SIZE;

        chk_msg(tio->buffer.size <= tio->buffer.capacity,
                "Expected less bunches. Buffer is not expected to grow.");
        chk_msg(mliTar_read_data(
                        &tio->tar,
                        (void *)(tio->buffer.bunches),
                        tio->tarh.size),
                "Failed to read cherenkov_bunches from block into buffer.");
        return 1;
error:
        return 0;
}

int mliTarIoReader_read_cherenkov_bunch(
        struct mliTarIoReader *tio,
        float *bunch)
{
        int i;

        if (tio->buffer.bunches == NULL) {
                if (!tio->has_tarh) {
                        return 0;
                }
                chk_msg(mliTarIoReader_tarh_is_valid_cherenkov_block(tio),
                        "Cherenkov-bunch-block's tar-header doesn't match.");
                chk_msg(mliTarIoReader_malloc_buffer(tio),
                        "Can't malloc cherenkov-bunch-block-bluffer.");
                chk_msg(mliTarIoReader_read_buffer(tio),
                        "Can't read first cherenkov-bunch-block.");
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
                tio->cherenkov_bunch_block_number += 1;
                tio->buffer_at = 0;
        }

        if (tio->buffer_at == tio->buffer.size) {
                if (!tio->has_tarh) {
                        return 0;
                }
                if (!mliTarIoReader_tarh_might_be_valid_cherenkov_block(tio)) {
                        return 0;
                }
                chk_msg(mliTarIoReader_tarh_is_valid_cherenkov_block(tio),
                        "Cherenkov-bunch-block's tar-header doesn't match.");
                chk_msg(mliTarIoReader_read_buffer(tio),
                        "Can't read first cherenkov-bunch-block.");
                tio->has_tarh = mliTar_read_header(&tio->tar, &tio->tarh);
                tio->cherenkov_bunch_block_number += 1;
                tio->buffer_at = 0;
        }

        if (tio->buffer.size == 0) {
                return 0;
        }

        for (i = 0; i < 8; i++) {
                bunch[i] = tio->buffer.bunches[tio->buffer_at * 8 + i];
        }
        tio->buffer_at += 1;
        tio->bunch_number += 1;

        return 1;
error:
        return 0;
}

int mliTarIoReader_close(struct mliTarIoReader *tio)
{
        mliTarIoCherenkovBunchBuffer_free(&tio->buffer);
        chk_msg(mliTar_close(&tio->tar), "Can't close tar-file.");
        return 1;
error:
        return 0;
}
