/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "frame_from_archive.h"
#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "../json/json.h"
#include "../cstr/cstr.h"
#include "../io/io_text.h"
#include "../frame/frame_json.h"

int mli_Frame_from_Archive(
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
