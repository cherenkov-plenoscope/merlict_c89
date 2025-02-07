/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRYTOMATERIALMAP_EQUAL_H_
#define MLI_GEOMETRYTOMATERIALMAP_EQUAL_H_

#include "../bool/bool.h"
#include "geometrytomaterialmap.h"

mli_bool mli_GeometryToMaterialMap_equal(
        const struct mli_GeometryToMaterialMap *a,
        const struct mli_GeometryToMaterialMap *b);
#endif
