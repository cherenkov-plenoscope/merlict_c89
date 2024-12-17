/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRYTOMATERIALMAP_EQUAL_H_
#define MLIGEOMETRYTOMATERIALMAP_EQUAL_H_

#include "mliGeometryToMaterialMap.h"

int mliGeometryToMaterialMap_valid(
        const struct mliGeometryToMaterialMap *geomap);

int mliGeometryToMaterialMap_valid_wrt_Geometry(
        const struct mliGeometryToMaterialMap *geomap,
        const struct mliGeometry *geometry);
int mliGeometryToMaterialMap_valid_wrt_Materials(
        const struct mliGeometryToMaterialMap *geomap,
        const struct mli_Materials *materials);
#endif
