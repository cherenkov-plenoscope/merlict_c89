/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_from_archive.h"
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "../geometry/geometry.h"
#include "../json/json.h"
#include "../json/json_walk.h"
#include "../cstr/cstr.h"
#include "../io/io_text.h"
#include "../path/path.h"
#include "../archive/archive.h"
#include "../frame/frame.h"
#include "../frame/frame_json.h"
#include "../func/func_csv.h"

int mli_Materials__key_from_filename(
        struct mli_String *key,
        const struct mli_String *filename)
{
        struct mli_String basename = mli_String_init();
        struct mli_String extension = mli_String_init();
        chk(mli_path_basename(filename, &basename));
        chk(mli_path_splitext(&basename, key, &extension));
        mli_String_free(&extension);
        mli_String_free(&basename);
        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive__set_spectra(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{
        uint64_t spc_idx = 0;
        uint64_t arc_idx = 0;

        struct mli_String *filename = NULL;
        struct mli_String key = mli_String_init();

        spc_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                filename = &archive->filenames.items.array[arc_idx].key;

                if (mli_String_starts_with_cstr(
                            filename, "materials/spectra/") &&
                    mli_String_ends_with_cstr(filename, ".csv")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];
                        struct mli_IO buff = mli_IO_init();

                        chk_msg(spc_idx < materials->spectra.size,
                                "Expected sufficient capacity for spectra.");

                        chk(mli_FuncInfo_malloc(
                                &materials->spectra.array[spc_idx].info));
                        chk(mli_IO_open_memory(&buff));
                        chk(mli_IO_text_write_String(&buff, payload));
                        mli_IO_rewind(&buff);
                        chk_msg(mli_Func_from_csv(
                                        &materials->spectra.array[spc_idx]
                                                 .spectrum,
                                        &materials->spectra.array[spc_idx]
                                                 .info.x,
                                        &materials->spectra.array[spc_idx]
                                                 .info.y,
                                        &buff),
                                "Failed to parse spectral function from "
                                "archive.");
                        mli_IO_close(&buff);

                        chk(mli_Materials__key_from_filename(&key, filename));

                        chk_msg(mli_Map_insert(&names->spectra, &key, spc_idx),
                                "Failed to insert spectrum-name into map.");

                        chk(mli_String_copy(
                                &materials->spectra.array[spc_idx].name,
                                &names->spectra.items.array[spc_idx].key));
                        spc_idx += 1u;
                }
        }

        mli_String_free(&key);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive__set_media(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{
        uint64_t med_idx = 0;
        uint64_t arc_idx = 0;
        struct mli_String *filename = NULL;
        struct mli_String key = mli_String_init();

        med_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                filename = &archive->filenames.items.array[arc_idx].key;

                if (mli_String_starts_with_cstr(filename, "materials/media/") &&
                    mli_String_ends_with_cstr(filename, ".json")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];

                        chk_msg(med_idx < materials->media.size,
                                "Expected sufficient capacity for media.");

                        chk(mli_Materials__key_from_filename(&key, filename));

                        chk_msg(mli_Medium_from_json_string_and_name(
                                        &materials->media.array[med_idx],
                                        &names->spectra,
                                        payload,
                                        &key),
                                "Can't parse medium from json string.");

                        chk_msg(mli_Map_insert(&names->media, &key, med_idx),
                                "Failed to insert media-name into map.");

                        med_idx += 1u;
                }
        }

        chk_msgf(
                med_idx == materials->media.size,
                ("Expected to parse %lu media but only found %lu.",
                 materials->media.size,
                 med_idx));

        mli_String_free(&key);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive__set_surfaces(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{
        uint64_t srf_idx = 0;
        uint64_t arc_idx = 0;
        struct mli_String *filename = NULL;
        struct mli_String key = mli_String_init();

        srf_idx = 0u;
        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                filename = &archive->filenames.items.array[arc_idx].key;
                if (mli_String_starts_with_cstr(
                            filename, "materials/surfaces/") &&
                    mli_String_ends_with_cstr(filename, ".json")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];

                        chk_msg(srf_idx < materials->surfaces.size,
                                "Expected sufficient capacity for surfaces.");

                        chk(mli_Materials__key_from_filename(&key, filename));

                        chk_msg(mli_Surface_from_json_string_and_name(
                                        &materials->surfaces.array[srf_idx],
                                        &names->spectra,
                                        payload,
                                        &key),
                                "Can't parse surface from json string.");

                        chk_msg(mli_Map_insert(&names->surfaces, &key, srf_idx),
                                "Failed to insert surface-name into map.");

                        srf_idx += 1u;
                }
        }

        chk_msgf(
                srf_idx == materials->surfaces.size,
                ("Expected to parse %lu surfaces but only found %lu.",
                 materials->surfaces.size,
                 srf_idx));

        mli_String_free(&key);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer2_from_json_string_and_name(
        struct mli_BoundaryLayer2 *self,
        const struct mli_Map *surface_names,
        const struct mli_Map *media_names,
        const struct mli_String *json_string,
        const struct mli_String *name)
{
        struct mli_String key = mli_String_init();
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();

        chk_msg(mli_String_copy(&self->name, name),
                "Failed to copy boundary layer name.");
        chk_msg(mli_Json_from_string(&json, json_string),
                "Failed to parse boundary layer json string.");

        walk = mli_JsonWalk_set(&json);
        chk(mli_JsonWalk_to_key(&walk, "inner"));
        chk(mli_JsonWalk_to_key(&walk, "medium"));
        chk(mli_JsonWalk_get_string(&walk, &key));
        chk_msgf(
                mli_Map_get(media_names, &key, &self->inner.medium),
                ("Expected 'inner->medium':'%s' to be in media_names.",
                 key.array));

        walk = mli_JsonWalk_set(&json);
        chk(mli_JsonWalk_to_key(&walk, "inner"));
        chk(mli_JsonWalk_to_key(&walk, "surface"));
        chk(mli_JsonWalk_get_string(&walk, &key));
        chk_msgf(
                mli_Map_get(surface_names, &key, &self->inner.surface),
                ("Expected 'inner->surface':'%s' to be in surface_names.",
                 key.array));

        walk = mli_JsonWalk_set(&json);
        chk(mli_JsonWalk_to_key(&walk, "outer"));
        chk(mli_JsonWalk_to_key(&walk, "medium"));
        chk(mli_JsonWalk_get_string(&walk, &key));
        chk_msgf(
                mli_Map_get(media_names, &key, &self->outer.medium),
                ("Expected 'outer->medium':'%s' to be in media_names.",
                 key.array));

        walk = mli_JsonWalk_set(&json);
        chk(mli_JsonWalk_to_key(&walk, "outer"));
        chk(mli_JsonWalk_to_key(&walk, "surface"));
        chk(mli_JsonWalk_get_string(&walk, &key));
        chk_msgf(
                mli_Map_get(surface_names, &key, &self->outer.surface),
                ("Expected 'outer->surface':'%s' to be in surface_names.",
                 key.array));

        mli_Json_free(&json);
        mli_String_free(&key);
        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive__set_boundary_layers(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{
        uint64_t bdl_idx = 0;
        uint64_t arc_idx = 0;
        struct mli_String *filename = NULL;
        struct mli_String key = mli_String_init();

        for (arc_idx = 0u; arc_idx < mli_Archive_size(archive); arc_idx++) {
                filename = &archive->filenames.items.array[arc_idx].key;
                if (mli_String_starts_with_cstr(
                            filename, "materials/boundary_layers/") &&
                    mli_String_ends_with_cstr(filename, ".json")) {
                        struct mli_String *payload =
                                &archive->textfiles.array[arc_idx];

                        chk_msg(bdl_idx < materials->boundary_layers.size,
                                "Expected sufficient capacity for boundary "
                                "layers.");

                        chk(mli_Materials__key_from_filename(&key, filename));

                        chk_msg(mli_BoundaryLayer2_from_json_string_and_name(
                                        &materials->boundary_layers
                                                 .array[bdl_idx],
                                        &names->surfaces,
                                        &names->media,
                                        payload,
                                        &key),
                                "Can't set boundary layer from json string.");

                        chk_msg(mli_Map_insert(
                                        &names->boundary_layers, &key, bdl_idx),
                                "Failed to insert boundary layer name into "
                                "map.");

                        bdl_idx += 1u;
                }
        }

        chk_msgf(
                bdl_idx == materials->boundary_layers.size,
                ("Expected to parse %lu boundary layers but only found %lu.",
                 materials->boundary_layers.size,
                 bdl_idx));

        mli_String_free(&key);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive__set_default_medium(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{

        struct mli_String filename = mli_String_init();
        struct mli_String key = mli_String_init();
        struct mli_String *payload = NULL;

        chk(mli_String_from_cstr(&filename, "materials/default_medium.txt"));
        chk_msg(mli_Archive_get(archive, &filename, &payload),
                "Can not find 'materials/default_medium.txt' in scenery.");

        chk(mli_String_strip(payload, &key));
        chk_msg(mli_Map_get(&names->media, &key, &materials->default_medium),
                "Failed to assign the 'default_medium'.");

        mli_String_free(&filename);
        mli_String_free(&key);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_Archive(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive)
{
        uint64_t iii = 0;
        struct mli_IO ioerr = mli_IO_init();
        struct mli_MaterialsCapacity capacity = mli_MaterialsCapacity_init();

        /* free */
        /* ---- */
        mli_Materials_free(materials);
        mli_materials_Names_free(names);
        chk(mli_materials_Names_malloc(names));

        /* estimate capacity */
        /* ----------------- */
        capacity.num_boundary_layers = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/boundary_layers/", ".json");
        capacity.num_media = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/media/", ".json");
        capacity.num_surfaces = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/surfaces/", ".json");
        capacity.num_spectra = mli_Archive_num_filename_prefix_sufix(
                archive, "materials/spectra/", ".csv");
        chk_msg(mli_Materials_malloc(materials, capacity),
                "Can't malloc materials.");

        /* set fields */
        /* ---------- */
        chk_msg(mli_Materials_from_Archive__set_spectra(
                        materials, names, archive),
                "Can't set spectra from archive.");
        chk_msg(mli_Materials_from_Archive__set_media(
                        materials, names, archive),
                "Can't set media from archive.");
        chk_msg(mli_Materials_from_Archive__set_surfaces(
                        materials, names, archive),
                "Can't set surfaces from archive.");
        chk_msg(mli_Materials_from_Archive__set_boundary_layers(
                        materials, names, archive),
                "Can't set boundary layers from archive.");
        chk_msg(mli_Materials_from_Archive__set_default_medium(
                        materials, names, archive),
                "Can't set default_medium from archive.");

        chk(mli_IO_adopt_file(&ioerr, stderr));
        chk(mli_IO_text_write_cstr_format(&ioerr, "materials->spectra\n"));
        chk(mli_IO_text_write_cstr_format(&ioerr, "------------------\n"));
        for (iii = 0; iii < materials->spectra.size; iii++) {
                struct mli_Spectrum *spectrum = &materials->spectra.array[iii];
                chk(mli_Spectrum_print_to_io(spectrum, &ioerr));
        }

        return 1;
chk_error:
        mli_Materials_free(materials);
        mli_materials_Names_free(names);
        return 0;
}
