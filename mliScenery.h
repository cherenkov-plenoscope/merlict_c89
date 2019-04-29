/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_H_
#define MERLICT_MLISCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"

typedef struct {
    uint32_t num_functions;
    mliFunc *functions;
    uint32_t num_colors;
    mliColor *colors;
    mliMesh mesh;
    uint32_t *mesh_colors;
} mliScenery;


void mliScenery_free(mliScenery *scenery) {
    int i;
    for (i = 0; i < scenery->num_functions; i++) {
        mliFunc_free(&(scenery->functions[i]));
    }
    free(scenery->functions);
    scenery->num_functions = 0;

    free(scenery->colors);
    scenery->num_colors = 0;

    mliMesh_free(&scenery->mesh);

    free(scenery->mesh_colors);
}

#endif
