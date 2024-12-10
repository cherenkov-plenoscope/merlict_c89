/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "mliGeometry.h"
#include "../json/json.h"
#include "../cstr/cstr.h"
#include "mliUserScenery_json.h"
#include "mliArchive.h"
#include "mliObject_wavefront.h"
#include "mliMedium_json.h"
#include "mliFrame.h"
#include "mliFrame_json.h"

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
        chk_mem(mliDynMap_malloc(&namemap->media));
        chk_mem(mliDynMap_malloc(&namemap->surfaces));
        chk_mem(mliDynMap_malloc(&namemap->boundary_layers));
        return 1;
chk_error:
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
                            archive->filenames.items.array[arc_idx].key,
                            "geometry/objects/",
                            ".obj")) {

                        struct mli_IO buff = mli_IO_init();
                        chk(mli_IO_text_write_String(
                                &buff, &archive->textfiles.array[arc_idx]));
                        mli_IO_rewind(&buff);

                        chk_msg(obj_idx < geometry->num_objects,
                                "Expected less objects in archive.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
                                archive->filenames.items.array[arc_idx].key,
                                key);
                        mli_cstr_path_basename_without_extension(key, key);
                        chk_msg(mliDynMap_insert(object_names, key, obj_idx),
                                "Failed to insert object-filename into map.");

                        chk_msg(mliObject_malloc_from_wavefront(
                                        &geometry->objects[obj_idx], &buff),
                                "Failed to parse wave-front-object.");

                        memcpy(geometry->object_names[obj_idx].cstr,
                               key,
                               MLI_NAME_CAPACITY);

                        obj_idx += 1u;

                        mli_IO_close(&buff);
                }
        }

        return 1;
chk_error:
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

        struct mli_String *default_medium_text = NULL;
        struct mli_Json boundary_layers_json = mli_Json_init();
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
                            archive->filenames.items.array[arc_idx].key,
                            "materials/media/",
                            ".json")) {
                        chk_msg(mliMedium_malloc_from_json_str(
                                        &materials->media[med_idx],
                                        (char *)archive->textfiles
                                                .array[arc_idx]
                                                .array),
                                "Failed to parse media json from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
                                archive->filenames.items.array[arc_idx].key,
                                key);
                        mli_cstr_path_basename_without_extension(key, key);

                        chk_msg(mliDynMap_insert(&names->media, key, med_idx),
                                "Failed to insert media-name into map.");

                        memcpy(materials->medium_names[med_idx].cstr,
                               names->media.items.array[med_idx].key,
                               MLI_NAME_CAPACITY);

                        med_idx += 1u;
                }
        }

        /* surfaces */
        srf_idx = 0u;
        for (arc_idx = 0u; arc_idx < mliArchive_num(archive); arc_idx++) {
                if (mli_cstr_has_prefix_suffix(
                            archive->filenames.items.array[arc_idx].key,
                            "materials/surfaces/",
                            ".json")) {
                        chk_msg(mliSurface_malloc_from_json_str(
                                        &materials->surfaces[srf_idx],
                                        (char *)archive->textfiles
                                                .array[arc_idx]
                                                .array),
                                "Failed to parse surface json from "
                                "file.");

                        memset(key, '\0', sizeof(key));
                        mli_cstr_path_basename_without_extension(
                                archive->filenames.items.array[arc_idx].key,
                                key);
                        mli_cstr_path_basename_without_extension(key, key);

                        chk_msg(mliDynMap_insert(
                                        &names->surfaces, key, srf_idx),
                                "Failed to insert surface-name into map.");

                        memcpy(materials->surface_names[srf_idx].cstr,
                               names->surfaces.items.array[srf_idx].key,
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
                memcpy(materials->boundary_layer_names[i].cstr,
                       names->boundary_layers.items.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        mli_Json_free(&boundary_layers_json);

        /* default medium */

        chk_msg(mliArchive_get(
                        archive,
                        "materials/default_medium.txt",
                        &default_medium_text),
                "Can not find 'materials/default_medium.txt' in scenery.");

        memset(key, '\0', sizeof(key));
        mli_cstr_strip_spaces((char *)default_medium_text->array, key);

        chk_msg(mliDynMap_get(&names->media, key, &materials->default_medium),
                "Failed to assign the 'default_medium'.");

        return 1;
chk_error:
        mli_Json_free(&boundary_layers_json);
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
        struct mli_Json tree_json = mli_Json_init();
        chk_msg(mliArchive_get_malloc_json(
                        archive, "geometry/relations.json", &tree_json),
                "Failed to parse 'geometry/relations.json'.");
        chk_msg(mli_Json_token_by_key(&tree_json, 0, "children", &token),
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
        mli_Json_free(&tree_json);

        /* init transformations */
        mliFrame_set_frame2root(root);

        return 1;
chk_error:
        mli_Json_free(&tree_json);
        return 0;
}
