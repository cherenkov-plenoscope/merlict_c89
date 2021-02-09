/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_H_
#define MERLICT_C89_MLISCENERY_H_

#include "mliGeometry.h"
#include "mliMaterials.h"
#include "mliAccelerator.h"

struct mliScenery {
        struct mliGeometry geometry;
        struct mliAccelerator accelerator;
        struct mliMaterials materials;
};

struct mliScenery mliScenery_init(void);
void mliScenery_free(struct mliScenery *scenery);
#endif
