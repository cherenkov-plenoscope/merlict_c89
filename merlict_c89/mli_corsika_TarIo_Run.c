/* Copyright 2020 Sebastian A. Mueller*/
#include "mli_corsika_TarIo_Run.h"
#include "mli_corsika_utils.h"

struct mliTarIoRun mliTarIoRun_init(void)
{
        struct mliTarIoRun run;
        run.has_next_head = 0;
        run.next_head = mliTarHeader_init();
        run.tar = mliTar_init();
        memset(run.corsika_run_header, 0.0, 273);
        return run;
}

void mliTarIoRun_try_read_next_head(struct mliTarIoRun *run)
{
        run->has_next_head = mliTar_read_header(&run->tar, &run->next_head);
}

int mliTarIoRun_open(struct mliTarIoRun *run, const char *path)
{
        chk_msg(mliTar_open(&run->tar, path, "r"), "Failed to open tar.");

        /* corsika_run_header */
        /* ------------------ */
        mliTarIoRun_try_read_next_head(run);
        chk_msg(run->has_next_head, "Expected run to have tarinfo for runh.");
        chk_msg(strcmp(run->next_head.name, "runh.float32") == 0,
                "Expected first file to be 'runh.float32'.");
        chk_msg(run->next_head.size == 273 * sizeof(float),
                "Expected corsika_run_header to have size 273*sizeof(float).");
        chk_msg(mliTar_read_data(
                        &run->tar,
                        (void *)&(run->corsika_run_header),
                        run->next_head.size),
                "Failed to read corsika_run_header from tar.");
        chk_msg(run->corsika_run_header[0] == mli_chars_to_float("RUNH"),
                "Expected run->corsika_run_header[0] == 'RUNH'");

        /* next */
        /* ---- */
        mliTarIoRun_try_read_next_head(run);

        return 1;
error:
        return 0;
}

int mliTarIoRun_has_still_events_left(struct mliTarIoRun *run)
{
        return run->has_next_head;
}

int mliTarIoRun_close(struct mliTarIoRun *run)
{
        return mliTar_close(&run->tar);
}
