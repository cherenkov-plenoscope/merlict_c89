/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_H_
#define MERLICT_C89_MLISCENERY_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliVec.h"
#include "mli_primitive_codes.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliObject.h"
#include "mliSurface.h"
#include "mliMedium.h"
#include "mliBoundaryLayer.h"
#include "mliSceneryResources.h"

struct mliScenery {
        struct mliSceneryResources resources;

        uint32_t num_robjects;
        uint32_t *robjects;
        uint32_t *robject_user_ids;
        struct mliBoundaryLayer *robject_boundary_layers;
        struct mliHomTraComp *robject2root;
};

int mliScenery_malloc(struct mliScenery *scenery, const uint32_t num_robjects);
void mliScenery_free(struct mliScenery *scenery);
struct mliScenery mliScenery_init(void);
#endif
