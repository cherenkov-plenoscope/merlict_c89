/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISCENERY_H_
#define MLISCENERY_H_

#include "mliGeometry.h"
#include "mliGeometryToMaterialMap.h"
#include "mliMaterials.h"
#include "mliAccelerator.h"

struct mliScenery {
        struct mliGeometry geometry;
        struct mliAccelerator accelerator;
        struct mliMaterials materials;
        struct mliGeometryToMaterialMap geomap;
};

struct mliScenery mliScenery_init(void);
void mliScenery_free(struct mliScenery *scenery);
void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery);
#endif
