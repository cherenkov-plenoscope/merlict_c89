/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_from_archive.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "../geometry/geometry.h"
#include "../json/json.h"
#include "../cstr/cstr.h"
#include "../io/io_text.h"
#include "../path/path.h"
#include "../archive/archive.h"
#include "../object/object_wavefront.h"
#include "../frame/frame.h"
#include "../frame/frame_json.h"

chk_rc mli_Geometry_from_archive(
        struct mli_Geometry *geometry,
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
        return CHK_SUCCESS;
chk_error:
        mli_String_free(&extension);
        mli_String_free(&basename);
        mli_String_free(&key);
        return CHK_FAIL;
}
