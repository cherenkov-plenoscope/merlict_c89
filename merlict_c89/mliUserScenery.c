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

int mliUserScenery_malloc_from_tape_archive(
        struct mliUserScenery *uscn,
        const char *path)
{
        struct mliArchive arc = mliArchive_init();

        mli_check(
                mliArchive_malloc_from_tar(&arc, path),
                "Can not read tape-archive to malloc mliUserScenery.");

        mli_check(
                mliUserScenery_malloc_from_Archive(uscn, &arc),
                "Can not malloc mliUserScenery from archive.");

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

        uint64_t *arc_mask = NULL;
        uint64_t arc_idx = 0u;
        uint64_t obj_idx = 0u;
        uint64_t fnc_idx = 0u;
        uint64_t token = 0u;
        char key[128];

        /* free everything */
        /* --------------- */

        mliUserScenery_free(uscn);

        /* parse json files  */
        /*------------------ */

        mli_check(
                mliArchive_get_malloc_json(
                        arc, "materials.json", &materials_json),
                "Failed to parse 'materials.json'.");

        mli_check(
                mliArchive_get_malloc_json(arc, "tree.json", &scenery_json),
                "Failed to parse 'tree.json'.");

        /* estimate required capacity */
        /*--------------------------- */

        rescap.num_objects =
                mliArchive_num_filename_prefix_sufix(arc, "objects/", ".obj");

        rescap.num_functions =
                mliArchive_num_filename_prefix_sufix(arc, "functions/", ".csv");

        mli_check(
                __mliSceneryResourcesCapacity_from_materials_json(
                        &rescap, &materials_json),
                "Can not estimate capacity from materials-json.");

        /* malloc fields */
        /*-------------- */

        mli_check(
                mliSceneryResources_malloc(&uscn->resources, rescap),
                "Can not malloc resources in mliUserScenery.");

        mli_check(
                mliFrame_malloc(&uscn->root, MLI_FRAME),
                "Can not allocate root-frame in UserScenery.");

        mli_check(
                mliDynMap_malloc(
                        &uscn->object_names, uscn->resources.num_objects),
                "Failed to malloc object-name-map.");

        mli_check(
                mliDynMap_malloc(
                        &uscn->function_names, uscn->resources.num_functions),
                "Failed to malloc function-name-map.");

        mli_check(
                mliDynMap_malloc(
                        &uscn->color_names, uscn->resources.num_colors),
                "Failed to malloc color-name-map.");

        mli_check(
                mliDynMap_malloc(
                        &uscn->medium_names, uscn->resources.num_media),
                "Failed to malloc media-name-map.");

        mli_check(
                mliDynMap_malloc(
                        &uscn->surface_names, uscn->resources.num_surfaces),
                "Failed to malloc surface-name-map.");

        /* set fields */
        /* ---------- */

        /* objects */
        mli_malloc(arc_mask, uint64_t, mliArchive_num(arc));

        mliArchive_mask_filename_prefix_sufix(
                arc, arc_mask, "objects/", ".obj");

        obj_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(arc); arc_idx++) {
                if (arc_mask[arc_idx]) {

                        mli_check(
                                obj_idx < uscn->resources.num_objects,
                                "Expected less objects in archive.");

                        mli_check(
                                mliObject_malloc_from_wavefront(
                                        &uscn->resources.objects[obj_idx],
                                        arc->strings.arr[arc_idx].c_str),
                                "Failed to parse wave-front-object from file.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(arc->filenames.arr[arc_idx].key, key);

                        mli_check(
                                mliDynMap_insert(
                                        &uscn->object_names, key, obj_idx),
                                "Failed to insert object-filename into map.");

                        obj_idx += 1u;
                }
        }

        /* functions */
        mliArchive_mask_filename_prefix_sufix(
                arc, arc_mask, "functions/", ".csv");

        fnc_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(arc); arc_idx++) {
                if (arc_mask[arc_idx]) {

                        mli_check(
                                fnc_idx < uscn->resources.num_functions,
                                "Expected less functions in archive.");

                        mli_check(
                                mliFunc_malloc_from_string(
                                        &uscn->resources.functions[fnc_idx],
                                        arc->strings.arr[arc_idx].c_str),
                                "Failed to parse comma-separated-values from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(arc->filenames.arr[arc_idx].key, key);

                        mli_check(
                                mliDynMap_insert(
                                        &uscn->function_names, key, fnc_idx),
                                "Failed to insert function-filename into map.");

                        fnc_idx += 1u;
                }
        }

        free(arc_mask);
        arc_mask = NULL;

        /* colors */

        mli_check(
                __mliScenery_assign_colors_from_json(
                        &uscn->resources, &uscn->color_names, &materials_json),
                "Failed to copy colors from materials.json.");

        /* media */

        mli_check(
                __mliScenery_assign_media_from_json(
                        &uscn->resources,
                        &uscn->medium_names,
                        &uscn->function_names,
                        &materials_json),
                "Failed to copy media from materials.json.");

        /* surfaces */

        mli_check(
                __mliScenery_assign_surfaces_from_json(
                        &uscn->resources,
                        &uscn->surface_names,
                        &uscn->function_names,
                        &uscn->color_names,
                        &materials_json),
                "Failed to copy surfaces from materials.json.");

        /* default medium */

        mli_check(
                mliJson_find_key(&materials_json, 0, "default_medium", &token),
                "Expected materials.json to have key 'default_medium'.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        &uscn->medium_names,
                        &materials_json,
                        token,
                        &uscn->resources.default_medium),
                "Failed to assign the 'default_medium'.");

        mliJson_free(&materials_json);

        /* frames */

        mli_check(
                mliJson_find_key(&scenery_json, 0, "children", &token),
                "Expected scenery-json to have key 'children'.");
        mli_check(
                __mliFrame_from_json(
                        &uscn->root,
                        &scenery_json,
                        token + 1,
                        &uscn->object_names,
                        &uscn->surface_names,
                        &uscn->medium_names),
                "Failed to populate tree of Frames from scenery-json.");

        mliJson_free(&scenery_json);

        /* transformations */
        mliFrame_set_frame2root(&uscn->root);

        return 1;
error:
        mliJson_free(&materials_json);
        mliJson_free(&scenery_json);
        free(arc_mask);

        mliUserScenery_free(uscn);
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

void __mli_strip_key(const char *filename, char *key)
{
        uint64_t i = 0u;
        uint64_t o = 0u;

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '/') {
                        i += 1;
                        break;
                }
                i += 1;
        }

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '.') {
                        i += 1;
                        break;
                }
                key[o] = filename[i];
                i += 1;
                o += 1;
        }

finalize:
        key[o] = '\0';
}
