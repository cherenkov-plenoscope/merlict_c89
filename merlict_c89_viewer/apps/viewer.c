/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict_c89_viewer/apps/viewer.c -o viewer -lm
 */

#include <stdio.h>

#include "../../merlict_c89/all_headers.h"
#include "../../merlict_c89/all_sources.c"
#include "../../merlict_c89_viewer/all_headers.h"
#include "../../merlict_c89_viewer/all_sources.c"

int main(int argc, char *argv[])
{
        struct mlivrConfig config = mlivrConfig_default();
        struct mliScenery scenery = mliScenery_init();

        if (argc >= 2) {
                if (mli_string_ends_with(argv[1], ".tar")) {
                        chk_msg(
                                mliScenery_malloc_from_tar(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.tar'.");
                } else if (mli_string_ends_with(argv[1], ".mli")) {
                        chk_msg(
                                mliScenery_malloc_from_path(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.mli'.");
                } else if (mli_string_ends_with(argv[1], ".obj")) {
                        chk_msg(
                                mliScenery_malloc_minimal_from_wavefront(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.obj'.");
                } else {
                        chk_bad("Scenery-format has to be either of "
                                "('.tar', '.mli', '.obj').");
                }
        }

        if (argc == 3) {
                chk_msg(
                        mli_cstr_to_double(&config.step_length, argv[2]),
                        "Can not parse step_length from argv[2].");
                config.aperture_camera_image_sensor_width *= config.step_length;
        }

        if (argc < 2 || argc > 3) {
                fprintf(stderr,
                        "Usage: %s <scenery-path> [step_length]\n",
                        argv[0]);
                goto error;
        }

        chk_msg(
                mlivr_run_interactive_viewer_try_non_canonical_stdin(
                        &scenery, config),
                "Failure in viewer");

        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}
