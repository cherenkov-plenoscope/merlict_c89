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
        struct mliAccelerator accelerator = mliAccelerator_init();
        struct mliCombine combine;

        combine.scenery = &scenery;
        combine.accelerator = &accelerator;

        if (argc >= 2) {
                mli_check(
                        mli_string_ends_with(argv[1], ".tar"),
                        "Expectec scenery-file to be a tape-archive.");

                mli_check(
                        mliScenery_malloc_from_tape_archive(
                                &scenery,
                                argv[1]),
                        "Can not read scenery from tape-archive.");

                mli_check(
                        mliAccelerator_malloc_from_scenery(
                                &accelerator,
                                &scenery),
                        "Failed to malloc accelerator form scenery.");
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

        /*mliAccelerator_fprint(stderr, combine.accelerator);*/

        mli_check(
                mlivr_run_interactive_viewer(&combine, config),
                "Failure in viewer");

        mliAccelerator_free(&accelerator);
        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}
