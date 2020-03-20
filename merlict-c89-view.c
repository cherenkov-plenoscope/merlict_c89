/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict-c89-view.c -o merlict-c89-view -std=c89 -lm -Wall -pedantic
 */

#include <stdio.h>

#include "merlict_c89.h"
#include "merlict_c89_viewer.h"

int main(int argc, char *argv[]) {
    struct mliScenery scenery = mliScenery_init();
    struct mliOcTree octree = mliOcTree_init();

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <scenery-path>\n", argv[0]);
        goto error;
    }
    if (mli_string_ends_with(argv[1], ".json")) {
        mli_check(
            mliScenery_malloc_from_json_path(&scenery, argv[1]),
            "Can not read scenery from json.");
    } else {
        if (!mliScenery_read_from_path(&scenery, argv[1])) {
            fprintf(stderr, "Can not open '%s'\n", argv[1]);
            goto error;
        }
    }

    mli_check(mliOcTree_malloc_from_scenery(&octree, &scenery),
        "Failed to build octree from scenery.");
    mli_check(
        mlivr_run_interactive_viewer(&scenery, &octree, mlivrConfig_default()),
        "Failure in viewer");

    mliOcTree_free(&octree);
    mliScenery_free(&scenery);
    return EXIT_SUCCESS;
error:
    return EXIT_FAILURE;
}
