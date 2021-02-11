/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRYTOMATERIALMAP_EQUAL_H_
#define MERLICT_C89_MLIGEOMETRYTOMATERIALMAP_EQUAL_H_

#include "mliGeometryToMaterialMap.h"

int mliGeometryToMaterialMap_valid(
        const struct mliGeometryToMaterialMap *geomap);

int mliGeometryToMaterialMap_valid_wrt_Geometry(
        const struct mliGeometryToMaterialMap *geomap,
        const struct mliGeometry *geometry);
int mliGeometryToMaterialMap_valid_wrt_Materials(
        const struct mliGeometryToMaterialMap *geomap,
        const struct mliMaterials *materials);
#endif
