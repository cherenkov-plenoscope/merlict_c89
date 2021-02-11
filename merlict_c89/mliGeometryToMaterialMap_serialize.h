/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_
#define MERLICT_C89_MLIGEOMETRYTOMATERIALMAP_SERIALIZE_H_

#include <stdio.h>
#include "mliGeometryToMaterialMap.h"

int mliGeometryToMaterialMap_malloc_fread(
        struct mliGeometryToMaterialMap *geomap,
        FILE *f);
int mliGeometryToMaterialMap_fwrite(
        const struct mliGeometryToMaterialMap *geomap,
        FILE *f);
#endif
