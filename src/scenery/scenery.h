/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_H_
#define MLI_SCENERY_H_

#include "../geometry/geometry.h"
#include "../mli/mliGeometryToMaterialMap.h"
#include "../materials/materials.h"
#include "../accelerator/accelerator.h"

struct mliScenery {
        struct mli_Geometry geometry;
        struct mli_Accelerator accelerator;
        struct mli_Materials materials;
        struct mli_GeometryToMaterialMap geomap;
};

struct mliScenery mliScenery_init(void);
void mliScenery_free(struct mliScenery *scenery);
void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery);
#endif
