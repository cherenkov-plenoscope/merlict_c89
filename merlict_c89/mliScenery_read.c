/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_read.h"

int _mliScenery_read_user_ids(struct mliScenery *scenery, FILE *f)
{
        mli_fread(
                scenery->user_ids,
                sizeof(uint32_t),
                scenery->num_primitives,
                f);
        return 1;
error:
        return 0;
}

int mliScenery_read_from_path(struct mliScenery *scenery, const char *path)
{
        char line[1024];
        struct mliSceneryCapacity scenery_capacity = mliSceneryCapacity_init();
        struct mliSceneryResourcesCapacity resource_capacity =
                mliSceneryResourcesCapacity_init();
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open Scenery-file for reading.");

        /* identifier */
        mli_check(fgets(line, 1024, f), "Can not read identifier 1st line.");
        mli_check(
                strcmp(line, "merlict_c89\n") == 0,
                "Expected starts with 'merlict_c89\\n'.");
        mli_check(fgets(line, 1024, f), "Can not read identifier 2nd line.");
        mli_check(
                strncmp(line, "MLI_VERSION", 11) == 0,
                "Expected starts with 'MLI_VERSION'.");
        mli_check(fgets(line, 1024, f), "Can not read identifier 3rd line.");
        mli_check(
                strcmp(line, "scenery\n") == 0,
                "Expected starts with 'scenery\\n'.");

        mli_check(
                mliScenery_read_capacity_from_file(&scenery_capacity, f),
                "Failed to read Scenery's capacity from file");

        mli_check(
                mliSceneryResources_read_capacity_from_file(
                        &resource_capacity, f),
                "Failed to read SceneryResources' capacity from file");

        mli_check_mem(mliScenery_malloc(scenery, scenery_capacity));
        mli_check_mem(mliSceneryResources_malloc(
                &scenery->resources, resource_capacity));
        mli_check(
                mliSceneryResources_read_from_file(&scenery->resources, f),
                "Failed to read resources from file.");

        mli_fread(&scenery->default_medium, sizeof(uint32_t), 1u, f);

        mli_c(_mliScenery_read_vertices_and_triangles(scenery, f));

        mli_c(_mliScenery_read_user_ids(scenery, f));

        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        mliScenery_free(scenery);
        return 0;
}
