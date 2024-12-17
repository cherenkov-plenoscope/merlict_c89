/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYTOMATERIALMAP_SERIALIZE_H_
#define MLI_GEOMETRYTOMATERIALMAP_SERIALIZE_H_

#include "../io/io.h"
#include "geometrytomaterialmap.h"

int mli_GeometryToMaterialMap_from_io(
        struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
int mli_GeometryToMaterialMap_to_io(
        const struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
#endif
