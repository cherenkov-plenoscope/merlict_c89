/* Copyright 2019 Sebastian Achim Mueller                                     */

#include <stdio.h>
#include <stdlib.h>
#include "../../src/viewer/Config.h"
#include "../../src/viewer/viewer.h"
#include "../../src/chk/chk.h"
#include "../../src/cstr/cstr.h"
#include "../../src/cstr/cstr_numbers.h"
#include "../../src/scenery/scenery.h"
#include "../../src/scenery/scenery_tar.h"
#include "../../src/scenery/scenery_minimal_object.h"
#include "../../src/scenery/scenery_serialize.h"
#include "../../src/args/args.h"
#include "../../src/string/string_numbers.h"

int main(int argc, char *argv[])
{
        struct mli_StringVector args = mli_StringVector_init();
        struct mli_viewer_Config config = mli_viewer_Config_default();
        struct mli_Scenery scenery = mli_Scenery_init();

        chk_msg(
                mli_StringVector_from_argc_argv(&args, argc, argv),
                "Failed to copy argc and argv."
        );

        if (args.size >= 2) {
                if (mli_String_ends_with_cstr(&args.array[1], ".tar")) {
                        chk_msg(mli_Scenery__from_path_cstr(
                                        &scenery,
                                        args.array[1].array),
                                "Can not read scenery from '.tar'.");
                } else if (mli_String_ends_with_cstr(&args.array[1], ".mli")) {
                        chk_msg(mli_Scenery_malloc_from_path(
                                &scenery,
                                args.array[1].array),
                                "Can not read scenery from '.mli'.");
                } else if (mli_String_ends_with_cstr(&args.array[1], ".obj")) {
                        chk_msg(mli_Scenery_malloc_minimal_from_wavefront(
                                        &scenery,
                                        args.array[1].array),
                                "Can not read scenery from '.obj'.");
                } else {
                        chk_bad("Scenery-format has to be either of "
                                "('.tar', '.mli', '.obj').");
                }
        }

        if (args.size == 3) {
                chk_msg(mli_String_to_double(
                        &config.step_length,
                        &args.array[2]),
                        "Can not parse step_length from argv[2].");
                config.aperture_camera_image_sensor_width *= config.step_length;
        }

        if (args.size < 2 || args.size > 3) {
                fprintf(stderr,
                        "Usage: %s <scenery-path> [step_length]\n",
                        args.array[0].array);
                goto chk_error;
        }

        chk_msg(mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
                        &scenery, config),
                "Failure in viewer");

        mli_StringVector_free(&args);
        mli_Scenery_free(&scenery);
        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}
