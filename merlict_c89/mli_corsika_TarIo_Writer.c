/* Copyright 2020 Sebastian A. Mueller */
#include "mli_corsika_TarIo_Writer.h"
#include "mli_corsika_utils.h"
#include "mliTar.h"

/* buffer */
struct mliTarIoCherenkovBunchBuffer mliTarIoCherenkovBunchBuffer_init(void)
{
        struct mliTarIoCherenkovBunchBuffer buffer;
        buffer.num_bunches_capacity = 0;
        buffer.num_bunches = 0;
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
        const uint64_t num_bunches_capacity)
{
        mliTarIoCherenkovBunchBuffer_free(b);
        chk_msg(num_bunches_capacity > 0, "Expected buffer size > 0");
        b->num_bunches_capacity = num_bunches_capacity;
        chk_malloc(
                b->bunches,
                float,
                b->num_bunches_capacity *MLI_TARIO_CORSIKA_BUNCH_SIZE);
        return 1;
error:
        return 0;
}

/* file structure */
struct mliTarIoWriter mliTarIoWriter_init(void)
{
        struct mliTarIoWriter tio;
        tio.tar = mliTar_init();
        tio.event_number = 1;
        tio.cherenkov_bunch_block_number = 1;
        tio.buffer = mliTarIoCherenkovBunchBuffer_init();
        return tio;
}

int mliTarIoWriter_open(
        struct mliTarIoWriter *tio,
        const char *path,
        const int num_bunches_capacity)
{
        struct mliTarHeader tarh = mliTarHeader_init();
        char readme_md[] = "Tape-Archive-Io for CORSIKA\n"
                           "===========================\n"
                           "\n"
                           "Stores the Cherenkov light emitted by airshowers.\n"
                           "\n"
                           "Structure\n"
                           "---------\n"
                           " |-> auxillary.1          < Files like README\n"
                           " |-> auxillary.N\n"
                           " |\n"
                           " |-> RUNH.float32         < Start run\n"
                           " +-> 000000001            < Actuall event-number\n"
                           " | |-> EVTH.float32       < Start event\n"
                           " | |-> 000000001.float32  < First Cherenkov-block\n"
                           " | |-> 000000002.float32\n"
                           " | |-> .\n"
                           " | |-> .\n"
                           " | |-> .\n"
                           " | |-> 123456788.float32\n"
                           " | |-> 123456789.float32  < Last herenkov-block\n"
                           " |\n"
                           " +-> 000000002\n"
                           "   |-> EVTH.float32\n"
                           "   |-> 000000001.float32\n"
                           "   |-> 000000002.float32\n"
                           "   |-> .\n"
                           "   |-> .\n"
                           "   |-> .\n"
                           "   |-> 123456788.float32\n"
                           "   |-> 123456789.float32\n";

        chk_msg(mliTar_open(&tio->tar, path, "w"), "Can't open tar.");

        chk_msg(mliTarIoCherenkovBunchBuffer_malloc(
                        &tio->buffer, num_bunches_capacity),
                "Can't malloc cherenkov-bunch-buffer.");

        chk_msg(mliTarHeader_set_normal_file(
                        &tarh, "README.md", strlen(readme_md)),
                "Can't set tar-header for readme.md.");
        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for readme.md to tar.");
        chk_msg(mliTar_write_data(
                        &tio->tar,
                        readme_md,
                        strlen(readme_md)),
                "Can't write data of readme.md to tar.");
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
        if (tio->buffer.num_bunches > 0) {
                chk_msg(mliTarIoWriter_finalize_cherenkov_bunch_block(tio),
                        "Can't finalize previous event's cherenkov-bunch-block");
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

        chk_msg(tio->buffer.num_bunches > 0, "Expected cherenkov buffer to")
        if (tio->buffer.num_bunches == 0) {
            return 1;
        }

        sprintf(path,
                "%09d/%09d.float32",
                tio->event_number,
                tio->cherenkov_bunch_block_number);

        chk_msg(mliTarHeader_set_normal_file(
                        &tarh,
                        path,
                        tio->buffer.num_bunches * MLI_TARIO_CORSIKA_BUNCH_SIZE),
                "Can't set cherenkov-bunch-block's tar-header.");

        chk_msg(mliTar_write_header(&tio->tar, &tarh),
                "Can't write tar-header for cherenkov-bunch-block to tar.");

        chk_msg(mliTar_write_data(
                        &tio->tar,
                        tio->buffer.bunches,
                        tio->buffer.num_bunches * MLI_TARIO_CORSIKA_BUNCH_SIZE),
                "Can't write cherenkov-bunch-block to tar-file.");
        tio->buffer.num_bunches = 0;

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
        if (tio->buffer.num_bunches == tio->buffer.num_bunches_capacity) {
                chk_msg(mliTarIoWriter_finalize_cherenkov_bunch_block(tio),
                        "Can't finalize cherenkov-bunch-block.");
                chk_msg(tio->buffer.num_bunches == 0, "Expected buffer to be empty.");
        }
        for (i = 0; i < 8; i ++) {
                tio->buffer.bunches[tio->buffer.num_bunches * 8 + i] = bunch[i];
        }
        tio->buffer.num_bunches += 1;
        return 1;
error:
        return 0;
}

int mliTarIoWriter_close(struct mliTarIoWriter *tio)
{
        if (tio->buffer.num_bunches > 0) {
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
