/* Copyright 2020 Sebastian A. Mueller*/
#include "../../merlict_c89_corsika/TarIo/Run.h"
#include "../../merlict_c89_corsika/utils.h"
#include "../../merlict_c89/mliTar.h"

struct mliTarIoRun mliTarIoRun_init(void)
{
        struct mliTarIoRun run;
        run.tar = mliTar_init();
        memset(run.corsika_run_header, 0.0, 273);
        return run;
}

int mliTarIoRun_open(struct mliTarIoRun *run, const char *path)
{
        struct mliTarHeader info = mliTarHeader_init();
        mli_check(mliTar_open(&run->tar, path, "r"), "Failed to open tar.");
        mli_check(
                mliTar_read_header(&run->tar, &info),
                "Failed to read tarinfo for corsika_run_header from tar.");
        mli_check(
                strcmp(info.name, "runh.float32") == 0,
                "Expected first file to be 'runh.float32'.");
        mli_check(
                info.size == 273 * sizeof(float),
                "Expected corsika_run_header to have size 273*sizeof(float).");
        mli_check(
                mliTar_read_data(
                        &run->tar, (void *)&(run->corsika_run_header), info.size),
                "Failed to read corsika_run_header from tar.");
        mli_check(
                run->corsika_run_header[0] == mli_4chars_to_float("RUNH"),
                "Expected run->corsika_run_header[0] == 'RUNH'");

        return 1;
error:
        return 0;
}

int mliTarIoRun_close(struct mliTarIoRun *run)
{
        return mliTar_close(&run->tar);
}
