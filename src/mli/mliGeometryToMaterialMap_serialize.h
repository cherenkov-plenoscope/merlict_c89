/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_
#define MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_

#include "../io/io.h"
#include "mliGeometryToMaterialMap.h"

int mliGeometryToMaterialMap_from_io(
        struct mliGeometryToMaterialMap *geomap,
        struct mli_IO *f);
int mliGeometryToMaterialMap_to_io(
        const struct mliGeometryToMaterialMap *geomap,
        struct mli_IO *f);
#endif
