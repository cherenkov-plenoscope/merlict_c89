/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_FROM_ARCHIVE_H_
#define MLI_GEOMETRY_FROM_ARCHIVE_H_

#include "../map/map.h"
#include "../chk/chk.h"

struct mli_Archive;
struct mli_Geometry;
chk_rc mli_Geometry_from_archive(
        struct mli_Geometry *geometry,
        struct mli_Map *object_names,
        const struct mli_Archive *archive);
#endif
