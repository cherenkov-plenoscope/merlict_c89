/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery.h"
#include <math.h>
#include <stdint.h>

struct mliScenery mliScenery_init(void)
{
        struct mliScenery scenery;
        scenery.geometry = mli_Geometry_init();
        scenery.accelerator = mli_Accelerator_init();
        scenery.materials = mli_Materials_init();
        scenery.geomap = mli_GeometryToMaterialMap_init();
        return scenery;
}

void mliScenery_free(struct mliScenery *scenery)
{
        mli_Geometry_free(&scenery->geometry);
        mli_Accelerator_free(&scenery->accelerator);
        mli_Materials_free(&scenery->materials);
        mli_GeometryToMaterialMap_free(&scenery->geomap);
}

void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery)
{
        mli_Geometry_info_fprint(f, &scenery->geometry);
        fprintf(f, "\n");
        mli_Accelerator_info_fprint(f, &scenery->accelerator);
        fprintf(f, "\n");
        mli_Materials_info_fprint(f, &scenery->materials);
        fprintf(f, "\n");
        mli_GeometryToMaterialMap_info_fprint(f, &scenery->geomap);
        fprintf(f, "\n");
}
