/* Copyright 2019 Sebastian Achim Mueller                                     */

#include <stdio.h>
#include <stdlib.h>
#include "../../src/viewer/Config.h"
#include "../../src/viewer/viewer.h"
#include "../../src/chk/chk.h"
#include "../../src/mtl/cstr.h"
#include "../../src/mtl/cstr_numbers.h"
#include "../../src/mli/mliScenery.h"
#include "../../src/mli/mliScenery_tar.h"
#include "../../src/mli/mliScenery_minimal_object.h"
#include "../../src/mli/mliScenery_serialize.h"

/*
        1) Almagamate
        -------------
        python ./tools/almagamate.py \
                build/almagamate \
                libs/mli \
                libs/mli_viewer \
                --header_path build/almagamate/mli-mli_viewer.h \
                --source_path build/almagamate/mli-mli_viewer.c

        2) Compile
        ----------
        gcc \
                -include build/almagamate/mli-mli_viewer.h \
                -include build/almagamate/mli-mli_viewer.c \
                libs/mli_viewer/apps/viewer.main.c \
                -o build/viewer \
                -lm
*/

int main(int argc, char *argv[])
{
        struct mli_viewer_Config config = mli_viewer_Config_default();
        struct mliScenery scenery = mliScenery_init();

        if (argc >= 2) {
                if (mli_cstr_ends_with(argv[1], ".tar")) {
                        chk_msg(mliScenery_malloc_from_path_tar(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.tar'.");
                } else if (mli_cstr_ends_with(argv[1], ".mli")) {
                        chk_msg(mliScenery_malloc_from_path(&scenery, argv[1]),
                                "Can not read scenery from '.mli'.");
                } else if (mli_cstr_ends_with(argv[1], ".obj")) {
                        chk_msg(mliScenery_malloc_minimal_from_wavefront(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.obj'.");
                } else {
                        chk_bad("Scenery-format has to be either of "
                                "('.tar', '.mli', '.obj').");
                }
        }

        if (argc == 3) {
                chk_msg(mli_cstr_to_double(&config.step_length, argv[2]),
                        "Can not parse step_length from argv[2].");
                config.aperture_camera_image_sensor_width *= config.step_length;
        }

        if (argc < 2 || argc > 3) {
                fprintf(stderr,
                        "Usage: %s <scenery-path> [step_length]\n",
                        argv[0]);
                goto chk_error;
        }

        chk_msg(mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
                        &scenery, config),
                "Failure in viewer");

        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}
