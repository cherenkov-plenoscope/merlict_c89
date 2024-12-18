/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_from_archive.h"
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "../geometry/geometry.h"
#include "../json/json.h"
#include "../cstr/cstr.h"
#include "../io/io_text.h"
#include "../path/path.h"
#include "../materials/materials_from_archive_json.h"
#include "../archive/archive.h"
#include "../medium/medium_json.h"
#include "../frame/frame.h"
#include "../frame/frame_json.h"
#include "../surface/surface_json.h"

int mli_Materials_from_Archive(
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
        chk_msg(mli_Materials_from_Archive__assign_boundary_layers_from_json(
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
