/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"
#include <math.h>
#include <stdint.h>
#include "mli_json.h"

struct mliNameMap mliNameMap_init(void)
{
        struct mliNameMap nm;
        nm.media = mliDynMap_init();
        nm.surfaces = mliDynMap_init();
        nm.boundary_layers = mliDynMap_init();
        return nm;
}

int mliNameMap_malloc(struct mliNameMap *namemap)
{
        mliNameMap_free(namemap);
        chk_mem(mliDynMap_malloc(&namemap->media, 0u));
        chk_mem(mliDynMap_malloc(&namemap->surfaces, 0u));
        chk_mem(mliDynMap_malloc(&namemap->boundary_layers, 0u));
        return 1;
error:
        return 0;
}

void mliNameMap_free(struct mliNameMap *namemap)
{
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
                if (mli_cstr_has_prefix_suffix(
                            archive->filenames.array[arc_idx].key,
                            "objects/",
                            ".obj")) {
                        chk_msg(obj_idx < geometry->num_objects,
                                "Expected less objects in archive.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
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

int mliMaterials_malloc_form_archive(
        struct mliMaterials *materials,
        struct mliNameMap *names,
        const struct mliArchive *archive)
{
        uint64_t i = 0u;
        uint64_t med_idx = 0;
        uint64_t srf_idx = 0;
        uint64_t arc_idx = 0;
        char key[MLI_NAME_CAPACITY];

        struct mliStr *default_medium_text = NULL;
        struct mliJson boundary_layers_json = mliJson_init();
        struct mliMaterialsCapacity cap = mliMaterialsCapacity_init();

        /* free */

        mliMaterials_free(materials);
        mliNameMap_free(names);

        chk(mliNameMap_malloc(names));

        /* estimate capacity */
        /* boundary_layers */

        chk_msg(mliArchive_get_malloc_json(
                        archive,
                        "materials/boundary_layers.json",
                        &boundary_layers_json),
                "Failed to parse 'materials/boundary_layers.json'.");

        chk_msg(boundary_layers_json.tokens[0].type == JSMN_OBJECT,
                "Expected key 'boundary_layers' to be a json-object.");
        cap.num_boundary_layers = boundary_layers_json.tokens[0].size;

        cap.num_media = mliArchive_num_filename_prefix_sufix(
                archive, "materials/media/", ".json");

        cap.num_surfaces = mliArchive_num_filename_prefix_sufix(
                archive, "materials/surfaces/", ".json");

        chk_msg(mliMaterials_malloc(materials, cap),
                "Can not malloc materials.");

        /* set fields */
        /* ---------- */

        /* media */
        med_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_cstr_has_prefix_suffix(
                            archive->filenames.array[arc_idx].key,
                            "materials/media/",
                            ".json")) {
                        chk_msg(mliMedium_malloc_from_json_str(
                                        &materials->media[med_idx],
                                        archive->textfiles.array[arc_idx]
                                                .c_str),
                                "Failed to parse media json from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
                                archive->filenames.array[arc_idx].key, key);
                        mli_cstr_path_basename_without_extension(key, key);

                        chk_msg(mliDynMap_insert(&names->media, key, med_idx),
                                "Failed to insert media-name into map.");

                        memcpy(materials->medium_names[med_idx].c_str,
                               names->media.array[med_idx].key,
                               MLI_NAME_CAPACITY);

                        med_idx += 1u;
                }
        }

        /* surfaces */
        srf_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_cstr_has_prefix_suffix(
                            archive->filenames.array[arc_idx].key,
                            "materials/surfaces/",
                            ".json")) {
                        chk_msg(mliSurface_malloc_from_json_str(
                                        &materials->surfaces[srf_idx],
                                        archive->textfiles.array[arc_idx]
                                                .c_str),
                                "Failed to parse surface json from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
                                archive->filenames.array[arc_idx].key, key);
                        mli_cstr_path_basename_without_extension(key, key);

                        chk_msg(mliDynMap_insert(
                                        &names->surfaces, key, srf_idx),
                                "Failed to insert surface-name into map.");

                        memcpy(materials->surface_names[srf_idx].c_str,
                               names->surfaces.array[srf_idx].key,
                               MLI_NAME_CAPACITY);

                        srf_idx += 1u;
                }
        }

        /* boundary_layers */
        chk_msg(mliMaterials_assign_boundary_layers_from_json(
                        materials,
                        &names->boundary_layers,
                        &names->surfaces,
                        &names->media,
                        &boundary_layers_json),
                "Failed to copy boundary_layers from materials.json.");
        for (i = 0; i < materials->num_boundary_layers; i++) {
                memcpy(materials->boundary_layer_names[i].c_str,
                       names->boundary_layers.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        mliJson_free(&boundary_layers_json);

        /* default medium */

        chk_msg(mliArchive_get(
                        archive,
                        "materials/default_medium.txt",
                        &default_medium_text),
                "Can not find 'materials/default_medium.txt' in scenery.");

        memset(key, '\0', sizeof(key));
        mli_cstr_strip_spaces(default_medium_text->c_str, key);

        chk_msg(mliDynMap_get(&names->media, key, &materials->default_medium),
                "Failed to assign the 'default_medium'.");

        return 1;
error:
        mliJson_free(&boundary_layers_json);
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
        chk_msg(mliFrame_from_json(
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
