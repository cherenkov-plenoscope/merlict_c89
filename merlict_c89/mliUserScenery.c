/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliJson.h"

struct mliNameMap mliNameMap_init(void)
{
        struct mliNameMap nm;
        nm.functions = mliDynMap_init();
        nm.colors = mliDynMap_init();
        nm.media = mliDynMap_init();
        nm.surfaces = mliDynMap_init();
        nm.boundary_layers = mliDynMap_init();
        return nm;
}

int mliNameMap_malloc(struct mliNameMap *namemap)
{
        mliNameMap_free(namemap);
        chk_mem(mliDynMap_malloc(&namemap->functions, 0u));
        chk_mem(mliDynMap_malloc(&namemap->colors, 0u));
        chk_mem(mliDynMap_malloc(&namemap->media, 0u));
        chk_mem(mliDynMap_malloc(&namemap->surfaces, 0u));
        chk_mem(mliDynMap_malloc(&namemap->boundary_layers, 0u));
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
        mliDynMap_free(&namemap->boundary_layers);
}

int mli_set_geometry_objects_and_names_from_archive(
        struct mliGeometry *geometry,
        struct mliDynMap *object_names,
        const struct mliArchive *archive)
{
        uint64_t arc_idx = 0u;
        uint64_t obj_idx = 0u;
        char key[MLI_NAME_CAPACITY];

        /* objects */
        obj_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_string_has_prefix_suffix(
                            archive->filenames.array[arc_idx].key,
                            "objects/",
                            ".obj")) {
                        chk_msg(obj_idx < geometry->num_objects,
                                "Expected less objects in archive.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(
                                archive->filenames.array[arc_idx].key, key);
                        chk_msg(mliDynMap_insert(object_names, key, obj_idx),
                                "Failed to insert object-filename into map.");
                        chk_msg(mliObject_malloc_from_wavefront(
                                        &geometry->objects[obj_idx],
                                        archive->textfiles.array[arc_idx]
                                                .c_str),
                                "Failed to parse wave-front-object.");
                        memcpy(geometry->object_names[obj_idx].c_str,
                               key,
                               MLI_NAME_CAPACITY);

                        obj_idx += 1u;
                }
        }

        return 1;
error:
        return 0;
}

int mli_check_malloc_materials_form_archive(
        struct mliMaterials *materials,
        struct mliNameMap *names,
        const struct mliArchive *archive)
{
        uint64_t i = 0u;
        uint64_t fnc_idx = 0;
        uint64_t arc_idx = 0;
        uint64_t token = 0u;
        char key[MLI_NAME_CAPACITY];

        struct mliJson materials_json = mliJson_init();
        struct mliMaterialsCapacity cap = mliMaterialsCapacity_init();

        /* free */
        mliMaterials_free(materials);
        mliNameMap_free(names);

        /* estimate capacity and malloc */
        chk(mliNameMap_malloc(names));

        chk_msg(mliArchive_get_malloc_json(
                        archive, "materials.json", &materials_json),
                "Failed to parse 'materials.json'.");

        cap.num_functions = mliArchive_num_filename_prefix_sufix(
                archive, "functions/", ".csv");

        chk_msg(__mliMaterialsCapacity_from_materials_json(
                        &cap, &materials_json),
                "Can not estimate capacity from materials-json.");

        chk_msg(mliMaterials_malloc(materials, cap),
                "Can not malloc materials.");

        /* set fields */
        /* ---------- */

        /* functions */
        fnc_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_string_has_prefix_suffix(
                            archive->filenames.array[arc_idx].key,
                            "functions/",
                            ".csv")) {
                        chk_msg(mliFunc_malloc_from_csv(
                                        &materials->functions[fnc_idx],
                                        archive->textfiles.array[arc_idx]
                                                .c_str),
                                "Failed to parse comma-separated-values from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        __mli_strip_key(
                                archive->filenames.array[arc_idx].key, key);

                        chk_msg(mliDynMap_insert(
                                        &names->functions, key, fnc_idx),
                                "Failed to insert function-name into map.");

                        memcpy(materials->function_names[fnc_idx].c_str,
                               names->functions.array[fnc_idx].key,
                               MLI_NAME_CAPACITY);

                        fnc_idx += 1u;
                }
        }

        /* colors */

        chk_msg(__mliMaterials_assign_colors_from_json(
                        materials, &names->colors, &materials_json),
                "Failed to copy colors from materials.json.");
        for (i = 0; i < materials->num_colors; i++) {
                memcpy(materials->color_names[i].c_str,
                       names->colors.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        /* media */

        chk_msg(__mliMaterials_assign_media_from_json(
                        materials,
                        &names->media,
                        &names->functions,
                        &materials_json),
                "Failed to copy media from materials.json.");
        for (i = 0; i < materials->num_media; i++) {
                memcpy(materials->medium_names[i].c_str,
                       names->media.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        /* surfaces */

        chk_msg(__mliMaterials_assign_surfaces_from_json(
                        materials,
                        &names->surfaces,
                        &names->functions,
                        &names->colors,
                        &materials_json),
                "Failed to copy surfaces from materials.json.");
        for (i = 0; i < materials->num_surfaces; i++) {
                memcpy(materials->surface_names[i].c_str,
                       names->surfaces.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        /* boundary_layers */

        chk_msg(__mliMaterials_assign_boundary_layers_from_json(
                        materials,
                        &names->boundary_layers,
                        &names->surfaces,
                        &names->media,
                        &materials_json),
                "Failed to copy boundary_layers from materials.json.");
        for (i = 0; i < materials->num_boundary_layers; i++) {
                memcpy(materials->boundary_layer_names[i].c_str,
                       names->boundary_layers.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        /* default medium */

        chk_msg(mliJson_find_key(&materials_json, 0, "default_medium", &token),
                "Expected materials.json to have key 'default_medium'.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
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

int mli_check_malloc_root_frame_from_Archive(
        struct mliFrame *root,
        const struct mliArchive *archive,
        const struct mliDynMap *object_names,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names)
{
        uint64_t token = 0u;
        struct mliJson tree_json = mliJson_init();
        chk_msg(mliArchive_get_malloc_json(archive, "tree.json", &tree_json),
                "Failed to parse 'tree.json'.");
        chk_msg(mliJson_find_key(&tree_json, 0, "children", &token),
                "Expected 'tree.json' to have key 'children'.");
        chk_msg(mliFrame_malloc(root, MLI_FRAME), "Can not malloc root-frame.");
        chk_msg(__mliFrame_from_json(
                        root,
                        &tree_json,
                        token + 1,
                        object_names,
                        objects,
                        boundary_layer_names),
                "Failed to populate tree of Frames from 'tree.json'.");
        mliJson_free(&tree_json);

        /* init transformations */
        mliFrame_set_frame2root(root);

        return 1;
error:
        mliJson_free(&tree_json);
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
