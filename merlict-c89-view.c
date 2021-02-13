/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict-c89-view.c -o merlict-c89-view -std=c89 -lm -Wall -pedantic
 */

#include <stdio.h>

#include "merlict_c89/merlict_c89.h"
#include "merlict_c89/merlict_c89.c"
#include "merlict_c89_viewer/merlict_c89_viewer.h"
#include "merlict_c89_viewer/merlict_c89_viewer.c"

int main(int argc, char *argv[])
{
        struct mlivrConfig config = mlivrConfig_default();
        struct mliScenery scenery = mliScenery_init();

        if (argc >= 2) {
                int rc = 0;
                if (!rc) {
                        rc = mliScenery_malloc_from_tar(&scenery, argv[1]);
                }
                if (!rc) {
                        rc = mliScenery_malloc_from_path(&scenery, argv[1]);
                }
                if (!rc) {
                        rc = mliScenery_malloc_minimal_from_wavefront(
                                &scenery, argv[1]);
                }
                mli_check(rc, "Failed to read scenery from scenery-path.")
        }

        if (argc == 3) {
                mli_check(
                        mli_string_to_float(&config.step_length, argv[2]),
                        "Can not parse step_length from argv[2].");
                config.aperture_camera_image_sensor_width *= config.step_length;
        }

        if (argc < 2 || argc > 3) {
                fprintf(stderr,
                        "Usage: %s <scenery-path> [step_length]\n",
                        argv[0]);
                goto error;
        }

        mli_check(
                mlivr_run_interactive_viewer_try_non_canonical_stdin(
                        &scenery, config),
                "Failure in viewer");

        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}
