/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISCENERY_H_
#define MLISCENERY_H_

#include "../geometry/geometry.h"
#include "mliGeometryToMaterialMap.h"
#include "../materials/materials.h"
#include "mliAccelerator.h"

struct mliScenery {
        struct mli_Geometry geometry;
        struct mliAccelerator accelerator;
        struct mli_Materials materials;
        struct mli_GeometryToMaterialMap geomap;
};

struct mliScenery mliScenery_init(void);
void mliScenery_free(struct mliScenery *scenery);
void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery);
#endif
