/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCombine.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliCombine mliCombine_init(void)
{
        struct mliCombine combine;
        combine.scenery = mliScenery_init();
        combine.accelerator = mliAccelerator_init();
        return combine;
}

void mliCombine_free(struct mliCombine *combine)
{
        mliScenery_free(&combine->scenery);
        mliAccelerator_free(&combine->accelerator);
}

int mliCombine_malloc_from_tar(
        struct mliCombine *combine,
        const char *path)
{
        struct mliArchive arc = mliArchive_init();

        mli_check(
                mliArchive_malloc_from_tar(&arc, path),
                "Can not read tape-archive to malloc mliCombine.");

        mli_check(
                mliCombine_malloc_from_archive(combine, &arc),
                "Can not malloc mliUserScenery from archive.");

        mliArchive_free(&arc);

        return 1;
error:
        return 0;
}

int mliCombine_malloc_from_archive(
        struct mliCombine *combine,
        const struct mliArchive *archive)
{
        struct mliUserScenery userscenery = mliUserScenery_init();

        mli_check(
                mliUserScenery_malloc_from_Archive(&userscenery, archive),
                "Failed to malloc mliUserScenery from mliArchive.");
        mli_check(
                mliScenery_malloc_from_mliUserScenery(
                        &combine->scenery, &userscenery),
                "Failed to malloc mliScenery from mliUserScenery.");

        mli_check(mliAccelerator_malloc_from_scenery(
                &combine->accelerator,
                &combine->scenery),
                "");

        mliUserScenery_free(&userscenery);
        return 1;
error:
        return 0;
}
