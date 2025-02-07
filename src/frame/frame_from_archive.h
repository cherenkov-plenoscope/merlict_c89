/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRAME_FROM_ARCHIVE_H_
#define MLI_FRAME_FROM_ARCHIVE_H_

#include "../chk/chk.h"
#include "../map/map.h"
#include "../archive/archive.h"

struct mli_Object;
struct mli_Frame;
chk_rc mli_Frame_from_Archive(
        struct mli_Frame *root,
        const struct mli_Archive *archive,
        const struct mli_Map *object_names,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names);
#endif
