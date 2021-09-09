/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery.h"
#include <math.h>
#include <stdint.h>

struct mliScenery mliScenery_init(void)
{
        struct mliScenery scenery;
        scenery.geometry = mliGeometry_init();
        scenery.accelerator = mliAccelerator_init();
        scenery.materials = mliMaterials_init();
        scenery.geomap = mliGeometryToMaterialMap_init();
        return scenery;
}

void mliScenery_free(struct mliScenery *scenery)
{
        mliGeometry_free(&scenery->geometry);
        mliAccelerator_free(&scenery->accelerator);
        mliMaterials_free(&scenery->materials);
        mliGeometryToMaterialMap_free(&scenery->geomap);
}

void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery)
{
        mliGeometry_info_fprint(f, &scenery->geometry);
        fprintf(f, "\n");
        mliAccelerator_info_fprint(f, &scenery->accelerator);
        fprintf(f, "\n");
        mliMaterials_info_fprint(f, &scenery->materials);
        fprintf(f, "\n");
        mliGeometryToMaterialMap_info_fprint(f, &scenery->geomap);
        fprintf(f, "\n");
}
