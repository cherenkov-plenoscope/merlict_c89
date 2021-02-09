/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliScenery mliScenery_init(void)
{
        struct mliScenery scenery;
        scenery.geometry = mliGeometry_init();
        scenery.accelerator = mliAccelerator_init();
        scenery.materials = mliMaterials_init();
        return scenery;
}

void mliScenery_free(struct mliScenery *scenery)
{
        mliGeometry_free(&scenery->geometry);
        mliAccelerator_free(&scenery->accelerator);
        mliMaterials_free(&scenery->materials);
}
