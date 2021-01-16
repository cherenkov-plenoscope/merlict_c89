/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"

/*
 *  A scenery as a user wants to define it.
 *  It can grow dynamically.
 *  It can represent the geometric hiracrhy between abstract frames.
 *  Before ray tracing, this UserScenery will be translated into a Scenery
 *  which is optimized for ray tracing.
 */

struct mliUserScenery mliUserScenery_init(void)
{
        struct mliUserScenery uscn;
        uscn.resources = mliSceneryResources_init();
        uscn.root = mliFrame_init();

        uscn.object_names = mliDynMap_init();
        uscn.function_names = mliDynMap_init();
        uscn.color_names = mliDynMap_init();
        uscn.medium_names = mliDynMap_init();
        uscn.surface_names = mliDynMap_init();
        return uscn;
}

void mliUserScenery_free(struct mliUserScenery *uscn)
{
        mliSceneryResources_free(&uscn->resources);
        mliFrame_free(&uscn->root);

        mliDynMap_free(&uscn->object_names);
        mliDynMap_free(&uscn->function_names);
        mliDynMap_free(&uscn->color_names);
        mliDynMap_free(&uscn->medium_names);
        mliDynMap_free(&uscn->surface_names);
        (*uscn) = mliUserScenery_init();
}

int mliUserScenery_malloc(struct mliUserScenery *uscn)
{
        mliUserScenery_free(uscn);
        mli_check(
                mliFrame_malloc(&uscn->root, MLI_FRAME),
                "Can not allocate root-frame in UserScenery.");
        return 1;
error:
        return 0;
}

int mliUserScenery_malloc_from_tape_archive(
        struct mliUserScenery *uscn,
        const char *path)
{
        struct mliArchive arc = mliArchive_init();

        mli_check(
                mliArchive_malloc_from_tar(&arc, path),
                "Can not read tape-archive to malloc mliUserScenery."
        );

        mli_check(
                mliUserScenery_malloc_from_Archive(uscn, &arc),
                "Can not malloc mliUserScenery from archive."
        );

        mliArchive_free(&arc);

        return 1;
error:
        return 0;
}

int mliUserScenery_malloc_from_Archive(
        struct mliUserScenery *uscn,
        const struct mliArchive *arc)
{
        struct mliJson materials_json = mliJson_init();
        struct mliJson scenery_json = mliJson_init();

        struct mliSceneryResourcesCapacity rescap =
                mliSceneryResourcesCapacity_init();

        mli_check(mliArchive_get_malloc_json(
                arc,
                "materials.json",
                &materials_json),
                "Can not parse materials.json.");

        mli_check(mliArchive_get_malloc_json(
                arc,
                "scenery.json",
                &scenery_json),
                "Can not parse scenery.json.");

        mli_check(mliSceneryResourcesCapacity_estimate(
                &rescap,
                arc,
                &materials_json),
                "Can not estimate capacity of archive.");

        mli_check(mliUserScenery_malloc(uscn), "Can not malloc mliUserScenery");

        mli_check(mliSceneryResources_malloc(&uscn->resources, rescap),
                "Can not malloc resources in mliUserScenery.");

        mliJson_free(&scenery_json);
        mliJson_free(&materials_json);

        return 1;
error:
        return 0;
}


int mliSceneryResourcesCapacity_estimate(
        struct mliSceneryResourcesCapacity *rescap,
        const struct mliArchive *arc,
        const struct mliJson *materials_json)
{
        rescap->num_objects = mliArchive_num_filename_prefix_sufix(
                arc, "objects/", ".obj");

        rescap->num_functions = mliArchive_num_filename_prefix_sufix(
                arc, "functions/", ".csv");

        mli_check(__mliSceneryResourcesCapacity_from_materials_json(
                rescap,
                materials_json),
                "Can not estimate capacity from materials-json.");

        return 1;
error:
        return 0;
}


int __mliSceneryResourcesCapacity_from_materials_json(
        struct mliSceneryResourcesCapacity *rescap,
        const struct mliJson *json)
{
        uint64_t token;
        mli_check(
                mliJson_find_key(json, 0, "colors", &token),
                "Expected materials-json to have key 'colors'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'colors' to point to a json-array.");
        rescap->num_colors = json->tokens[token + 1].size;

        mli_check(
                mliJson_find_key(json, 0, "media", &token),
                "Expected materials-json to have key 'media'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'media' to point to a json-array.");
        rescap->num_media = json->tokens[token + 1].size;

        mli_check(
                mliJson_find_key(json, 0, "surfaces", &token),
                "Expected materials-json to have key 'surfaces'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'surfaces' to point to a json-array.");
        rescap->num_surfaces = json->tokens[token + 1].size;
        return 1;
error:
        return 0;
}
