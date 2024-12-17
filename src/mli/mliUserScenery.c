/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "mliGeometry.h"
#include "../json/json.h"
#include "../cstr/cstr.h"
#include "../io/io_text.h"
#include "../path/path.h"
#include "mliUserScenery_json.h"
#include "../archive/archive.h"
#include "../object/object_wavefront.h"
#include "../medium/medium_json.h"
#include "../frame/frame.h"
#include "../frame/frame_json.h"
#include "../surface/surface_json.h"

int mli_set_geometry_objects_and_names_from_archive(
        struct mliGeometry *geometry,
        struct mli_Map *object_names,
        const struct mli_Archive *archive)
{
        uint64_t arc_idx = 0u;
        uint64_t obj_idx = 0u;
        struct mli_String key = mli_String_init();
        struct mli_String basename = mli_String_init();
        struct mli_String extension = mli_String_init();

        chk_dbg;
        /* objects */
        obj_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                struct mli_String *filename =
                        &archive->filenames.items.array[arc_idx].key;

                chk_dbg;
                if (mli_String_starts_with_cstr(
                            filename, "geometry/objects/") &&
                    mli_String_ends_with_cstr(filename, ".obj")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];
                        struct mli_IO buff = mli_IO_init();
                        chk_dbg;
                        chk(mli_IO_open_memory(&buff));
                        chk(mli_IO_text_write_String(&buff, payload));
                        mli_IO_rewind(&buff);

                        chk_dbg;
                        chk_msg(obj_idx < geometry->num_objects,
                                "Expected less objects in archive.");
                        chk_dbg;
                        chk(mli_path_basename(filename, &basename));
                        chk(mli_path_splitext(&basename, &key, &extension));

                        chk_msg(mli_Map_insert(object_names, &key, obj_idx),
                                "Failed to insert object-filename into map.");
                        chk_dbg;
                        chk_msg(mli_Object_malloc_from_wavefront(
                                        &geometry->objects[obj_idx], &buff),
                                "Failed to parse wave-front-object.");
                        chk_dbg;
                        chk(mli_String_copy(
                                &geometry->object_names[obj_idx], &key));
                        obj_idx += 1u;

                        mli_IO_close(&buff);
                }
        }
        chk_dbg;

        mli_String_free(&extension);
        mli_String_free(&basename);
        mli_String_free(&key);
        return 1;
chk_error:
        mli_String_free(&extension);
        mli_String_free(&basename);
        mli_String_free(&key);
        return 0;
}

int mli_Materials_malloc_form_archive(
        struct mli_Materials *materials,
        struct mli_ResourceNameMap *names,
        const struct mli_Archive *archive)
{
        uint64_t i = 0u;
        uint64_t med_idx = 0;
        uint64_t srf_idx = 0;
        uint64_t arc_idx = 0;

        struct mli_String *default_medium_text = NULL;
        struct mli_Json boundary_layers_json = mli_Json_init();
        struct mli_MaterialsCapacity cap = mli_MaterialsCapacity_init();

        struct mli_String *fixload = NULL;
        struct mli_String fixname = mli_String_init();
        struct mli_String key = mli_String_init();
        struct mli_String basename = mli_String_init();
        struct mli_String extension = mli_String_init();

        chk_dbg;
        /* free */
        mli_Materials_free(materials);
        chk_dbg;
        mli_ResourceNameMap_free(names);
        chk_dbg;
        chk(mli_ResourceNameMap_malloc(names));
        chk_dbg;
        /* estimate capacity */
        /* boundary_layers */
        chk_dbg;
        chk(mli_String_from_cstr(&fixname, "materials/boundary_layers.json"));
        chk_msg(mli_Archive_get(archive, &fixname, &fixload),
                "Can not find materials/boundary_layers.json file in archive.");
        chk_msg(mli_Json_from_string(&boundary_layers_json, fixload),
                "Failed to parse 'materials/boundary_layers.json'.");

        chk_dbg;
        chk_msg(boundary_layers_json.tokens[0].type == JSMN_OBJECT,
                "Expected key 'boundary_layers' to be a json-object.");
        cap.num_boundary_layers = boundary_layers_json.tokens[0].size;
        chk_dbg;
        cap.num_media = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/media/", ".json");
        chk_dbg;
        cap.num_surfaces = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/surfaces/", ".json");

        chk_msg(mli_Materials_malloc(materials, cap),
                "Can not malloc materials.");

        chk_dbg;
        /* set fields */
        /* ---------- */

        /* media */
        med_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                struct mli_String *filename =
                        &archive->filenames.items.array[arc_idx].key;

                if (mli_String_starts_with_cstr(filename, "materials/media/") &&
                    mli_String_ends_with_cstr(filename, ".json")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];
                        chk_msg(mli_Medium_malloc_from_json_string(
                                        &materials->media[med_idx], payload),
                                "Failed to parse media json from "
                                "file.");

                        chk(mli_path_basename(filename, &basename));
                        chk(mli_path_splitext(&basename, &key, &extension));

                        chk_msg(mli_Map_insert(&names->media, &key, med_idx),
                                "Failed to insert media-name into map.");

                        chk(mli_String_copy(
                                &materials->medium_names[med_idx],
                                &names->media.items.array[med_idx].key));
                        med_idx += 1u;
                }
        }

        chk_dbg;
        /* surfaces */
        srf_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                struct mli_String *filename =
                        &archive->filenames.items.array[arc_idx].key;
                if (mli_String_starts_with_cstr(
                            filename, "materials/surfaces/") &&
                    mli_String_ends_with_cstr(filename, ".json")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];
                        chk_msg(mli_Surface_malloc_from_json_string(
                                        &materials->surfaces[srf_idx], payload),
                                "Failed to parse surface json from "
                                "file.");

                        chk(mli_path_basename(filename, &basename));
                        chk(mli_path_splitext(&basename, &key, &extension));

                        chk_msg(mli_Map_insert(&names->surfaces, &key, srf_idx),
                                "Failed to insert surface-name into map.");

                        chk(mli_String_copy(
                                &materials->surface_names[srf_idx],
                                &names->surfaces.items.array[srf_idx].key));

                        srf_idx += 1u;
                }
        }
        chk_dbg;
        /* boundary_layers */
        chk_msg(mli_Materials_assign_boundary_layers_from_json(
                        materials,
                        &names->boundary_layers,
                        &names->surfaces,
                        &names->media,
                        &boundary_layers_json),
                "Failed to copy boundary_layers from materials.json.");

        for (i = 0; i < materials->num_boundary_layers; i++) {
                chk(mli_String_copy(
                        &materials->boundary_layer_names[i],
                        &names->boundary_layers.items.array[i].key));
        }
        chk_dbg;
        mli_Json_free(&boundary_layers_json);

        /* default medium */
        chk(mli_String_from_cstr(&fixname, "materials/default_medium.txt"));
        chk_msg(mli_Archive_get(archive, &fixname, &default_medium_text),
                "Can not find 'materials/default_medium.txt' in scenery.");

        chk(mli_String_strip(default_medium_text, &key));

        chk_msg(mli_Map_get(&names->media, &key, &materials->default_medium),
                "Failed to assign the 'default_medium'.");

        chk_dbg;
        mli_String_free(&fixname);
        mli_String_free(&extension);
        mli_String_free(&basename);
        mli_String_free(&key);
        return 1;
chk_error:
        mli_Json_free(&boundary_layers_json);
        mli_Materials_free(materials);
        mli_ResourceNameMap_free(names);

        mli_String_free(&fixname);
        mli_String_free(&extension);
        mli_String_free(&basename);
        mli_String_free(&key);
        return 0;
}

int mli_check_malloc_root_frame_from_Archive(
        struct mli_Frame *root,
        const struct mli_Archive *archive,
        const struct mli_Map *object_names,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names)
{
        uint64_t token = 0u;
        struct mli_String *fixload = NULL;
        struct mli_String fixname = mli_String_init();
        struct mli_Json tree_json = mli_Json_init();

        chk(mli_String_from_cstr(&fixname, "geometry/relations.json"));
        chk_msg(mli_Archive_get(archive, &fixname, &fixload),
                "Can not find geometry/relations.json file in archive.");
        chk_msg(mli_Json_from_string(&tree_json, fixload),
                "Failed to parse 'geometry/relations.json'.");

        chk_msg(mli_Json_token_by_key(&tree_json, 0, "children", &token),
                "Expected 'tree.json' to have key 'children'.");
        chk_msg(mli_Frame_malloc(root, MLI_FRAME_TYPE_FRAME),
                "Can not malloc root-frame.");
        chk_msg(mli_Frame_from_json(
                        root,
                        &tree_json,
                        token + 1,
                        object_names,
                        objects,
                        boundary_layer_names),
                "Failed to populate tree of Frames from 'tree.json'.");
        mli_Json_free(&tree_json);

        /* init transformations */
        mli_Frame_set_frame2root(root);

        mli_String_free(&fixname);
        return 1;
chk_error:
        mli_String_free(&fixname);
        mli_Json_free(&tree_json);
        return 0;
}
