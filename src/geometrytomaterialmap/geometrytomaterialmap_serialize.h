/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYTOMATERIALMAP_SERIALIZE_H_
#define MLI_GEOMETRYTOMATERIALMAP_SERIALIZE_H_

#include "../io/io.h"
#include "../chk/chk.h"
#include "geometrytomaterialmap.h"

chk_rc mli_GeometryToMaterialMap_from_io(
        struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
chk_rc mli_GeometryToMaterialMap_to_io(
        const struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
#endif
