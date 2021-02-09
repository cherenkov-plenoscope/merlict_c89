/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliJson.h"

struct mliNameMap mliNameMap_init(void) {
        struct mliNameMap nm;
        nm.functions = mliDynMap_init();
        nm.colors = mliDynMap_init();
        nm.media = mliDynMap_init();
        nm.surfaces = mliDynMap_init();
        return nm;
}

int mliNameMap_malloc(struct mliNameMap *namemap)
{
        mliNameMap_free(namemap);
        mli_check_mem(mliDynMap_malloc(&namemap->functions, 0u));
        mli_check_mem(mliDynMap_malloc(&namemap->colors, 0u));
        mli_check_mem(mliDynMap_malloc(&namemap->media, 0u));
        mli_check_mem(mliDynMap_malloc(&namemap->surfaces, 0u));
        return 1;
error:
        return 0;
}

void mliNameMap_free(struct mliNameMap *namemap)
{
        mliDynMap_free(&namemap->functions);
        mliDynMap_free(&namemap->colors);
        mliDynMap_free(&namemap->media);
        mliDynMap_free(&namemap->surfaces);
}

int mli_malloc_object_names_from_archive(
        struct mliDynMap *object_names,
        const struct mliArchive *archive)
{
        uint64_t arc_idx = 0u;
        uint64_t obj_idx = 0u;
        char key[MLI_NAME_CAPACITY];

        mliDynMap_malloc(object_names, 0u);

        /* objects */
        obj_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_string_has_prefix_suffix(
                        archive->filenames.arr[arc_idx].key, "objects/", ".obj")
                ) {
                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(
                                archive->filenames.arr[arc_idx].key,
                                key);
                        mli_check(
                                mliDynMap_insert(object_names, key, obj_idx),
                                "Failed to insert object-filename into map.");
                        obj_idx += 1u;
                }
        }

        return 1;
error:
        return 0;
}

int mli_malloc_materials_form_archive(
        struct mliMaterials *materials,
        struct mliNameMap *names,
        const struct mliArchive *archive)
{
        uint64_t fnc_idx = 0;
        uint64_t arc_idx = 0;
        uint64_t token = 0u;
        char key[MLI_NAME_CAPACITY];

        struct mliJson materials_json = mliJson_init();
        struct mliMaterialsCapacity cap =
                mliMaterialsCapacity_init();

        /* free */
        mliMaterials_free(materials);
        mliNameMap_free(names);

        /* estimate capacity and malloc */
        mli_c(mliNameMap_malloc(names));

        mli_check(
                mliArchive_get_malloc_json(
                        archive, "materials.json", &materials_json),
                "Failed to parse 'materials.json'.");

        cap.num_functions = mliArchive_num_filename_prefix_sufix(
                archive, "functions/", ".csv");

        mli_check(
                __mliMaterialsCapacity_from_materials_json(
                        &cap, &materials_json),
                "Can not estimate capacity from materials-json.");

        mli_check(
                mliMaterials_malloc(materials, cap),
                "Can not malloc materials.");

        /* set fields */
        /* ---------- */

        /* functions */
        fnc_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_string_has_prefix_suffix(
                        archive->filenames.arr[arc_idx].key, "functions/", ".csv")
                ) {
                        mli_check(
                                mliFunc_malloc_from_csv(
                                        &materials->functions[fnc_idx],
                                        archive->strings.arr[arc_idx].c_str),
                                "Failed to parse comma-separated-values from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(archive->filenames.arr[arc_idx].key, key);

                        mli_check(
                                mliDynMap_insert(
                                        &names->functions, key, fnc_idx),
                                "Failed to insert function-name into map.");

                        fnc_idx += 1u;
                }
        }

        /* colors */

        mli_check(
                __mliGeometry_assign_colors_from_json(
                        materials, &names->colors, &materials_json),
                "Failed to copy colors from materials.json.");

        /* media */

        mli_check(
                __mliGeometry_assign_media_from_json(
                        materials,
                        &names->media,
                        &names->functions,
                        &materials_json),
                "Failed to copy media from materials.json.");

        /* surfaces */

        mli_check(
                __mliGeometry_assign_surfaces_from_json(
                        materials,
                        &names->surfaces,
                        &names->functions,
                        &names->colors,
                        &materials_json),
                "Failed to copy surfaces from materials.json.");

        /* default medium */

        mli_check(
                mliJson_find_key(&materials_json, 0, "default_medium", &token),
                "Expected materials.json to have key 'default_medium'.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        &names->media,
                        &materials_json,
                        token,
                        &materials->default_medium),
                "Failed to assign the 'default_medium'.");

        mliJson_free(&materials_json);

        return 1;
error:
        mliJson_free(&materials_json);
        mliMaterials_free(materials);
        mliNameMap_free(names);
        return 0;
}

int mli_malloc_root_frame_from_Archive(
        struct mliFrame *root,
        const struct mliArchive *archive,
        const struct mliDynMap *object_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names)
{
        uint64_t token = 0u;
        struct mliJson tree_json = mliJson_init();
        mli_check(
                mliArchive_get_malloc_json(archive, "tree.json", &tree_json),
                "Failed to parse 'tree.json'.");
        mli_check(
                mliJson_find_key(&tree_json, 0, "children", &token),
                "Expected 'tree.json' to have key 'children'.");
        mli_check(
                mliFrame_malloc(root, MLI_FRAME),
                "Can not malloc root-frame.");
        mli_check(
                __mliFrame_from_json(
                        root,
                        &tree_json,
                        token + 1,
                        object_names,
                        surface_names,
                        medium_names),
                "Failed to populate tree of Frames from scenery-json.");
        mliJson_free(&tree_json);

        /* init transformations */
        mliFrame_set_frame2root(root);

        return 1;
error:
        mliJson_free(&tree_json);
        return 0;
}

int mliGeometry_set_objects_from_Archive(
        struct mliGeometry *scenery,
        const struct mliDynMap *object_names,
        const struct mliArchive *archive)
{
        uint64_t obj_idx = 0u;
        uint64_t obj_idx_in_name_map = 0u;
        uint64_t arc_idx = 0u;
        const uint64_t num_objects = object_names->dyn.size;
        char key[MLI_NAME_CAPACITY];

        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_string_has_prefix_suffix(
                        archive->filenames.arr[arc_idx].key, "objects/", ".obj")
                ) {
                        mli_check(
                                obj_idx < num_objects,
                                "Expected less objects in archive.");

                        mli_check(
                                mliObject_malloc_from_wavefront(
                                        &scenery->objects[obj_idx],
                                        archive->strings.arr[arc_idx].c_str),
                                "Failed to parse wave-front-object.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(archive->filenames.arr[arc_idx].key, key);
                        mli_check(mliDynMap_get(
                                object_names, key, &obj_idx_in_name_map),
                                "Failed to find object in object_names.");
                        mli_check(obj_idx == obj_idx_in_name_map,
                                "Expected a different obj_idx for this "
                                "object_name");
                        obj_idx += 1u;
                }
        }
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

int __mliMaterialsCapacity_from_materials_json(
        struct mliMaterialsCapacity *rescap,
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
