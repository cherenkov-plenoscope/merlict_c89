/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYTOMATERIALMAP_EQUAL_H_
#define MLI_GEOMETRYTOMATERIALMAP_EQUAL_H_

#include "geometrytomaterialmap.h"

int mli_GeometryToMaterialMap_valid(
        const struct mli_GeometryToMaterialMap *geomap);

int mli_GeometryToMaterialMap_valid_wrt_Geometry(
        const struct mli_GeometryToMaterialMap *geomap,
        const struct mli_Geometry *geometry);
int mli_GeometryToMaterialMap_valid_wrt_Materials(
        const struct mli_GeometryToMaterialMap *geomap,
        const struct mli_Materials *materials);
#endif
