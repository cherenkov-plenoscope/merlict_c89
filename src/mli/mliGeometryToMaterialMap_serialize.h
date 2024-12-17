/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_
#define MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_

#include "../io/io.h"
#include "mliGeometryToMaterialMap.h"

int mli_GeometryToMaterialMap_from_io(
        struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
int mli_GeometryToMaterialMap_to_io(
        const struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f);
#endif
