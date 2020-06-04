/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict-c89-view.c -o merlict-c89-view -std=c89 -lm -Wall -pedantic
 */

#include <stdio.h>

#include "merlict_c89.h"
#include "merlict_c89_viewer.h"

#include "mliRgbEfficiency.h"

int main(int argc, char *argv[])
{
        struct mliScenery scenery = mliScenery_init();
        struct mliOcTree octree = mliOcTree_init();
        struct mlivrConfig config = mlivrConfig_default();

        struct mliRgbEfficiency image_sensor = mliRgbEfficiency_init();
        struct mliLightSource sun = mliLightSource_init();
        struct mliColorMap colmap = mliColorMap_init();
        uint64_t num_optical_functions;

        sun.id = 0;
        sun.position = mliVec_set(1.e12, 1e12, 1e12);
        mli_check(mliLightSource_malloc_sunlight_spectrum(&sun),
                "Can not malloc sun.");

        mli_check(mliRgbEfficiency_malloc_default_cmos(&image_sensor),
                "Can not malloc default image-sensor.");

        if (argc >= 2) {
                if (mli_string_ends_with(argv[1], ".json")) {
                        mli_check(
                                mliScenery_malloc_from_json_path(
                                        &scenery,
                                        argv[1]),
                                "Can not read scenery from json.");
                } else {
                        if (!mliScenery_read_from_path(&scenery, argv[1])) {
                                fprintf(stderr, "Can not open '%s'\n", argv[1]);
                                goto error;
                        }
                }
        }

        if (argc == 3) {
                mli_check(
                        mli_string_to_float(&config.step_length, argv[2]),
                        "Can not parse step_length from argv[2].");
        }

        if (argc < 2 || argc > 3) {
                fprintf(
                        stderr,
                        "Usage: %s <scenery-path> [step_length]\n",
                        argv[0]);
                goto error;
        }

        mli_check(mliScenery_num_optical_functions(
                &scenery,
                &num_optical_functions),
                "Could not estimate number of optical functions in scenery.");

        mli_check(
                mliColorMap_malloc(
                        &colmap,
                        1,
                        num_optical_functions),
                "Can not malloc colormap.");

        mli_check(
                mliOcTree_malloc_from_scenery(&octree, &scenery),
                "Failed to build octree from scenery.");


        mli_check(
                mlivr_run_interactive_viewer(
                        &scenery,
                        &octree,
                        config),
                "Failure in viewer");


        mliRgbEfficiency_free(&image_sensor);
        mliLightSource_free(&sun);
        mliColorMap_free(&colmap);

        mliOcTree_free(&octree);
        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}
