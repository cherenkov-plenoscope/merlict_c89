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
        uint32_t default_medium;

        uint32_t num_objects;
        struct mliObject* objects;

        uint32_t num_robjects;
        uint32_t* robjects;
        struct mliBoundaryLayer *robject_boundary_layers;
        struct mliHomTraComp *robject_T;
        uint32_t *user_ids;
};

int mliScenery_malloc(struct mliScenery *s, const struct mliSceneryCapacity c);
int _mliScenery_malloc_user_ids(struct mliScenery *s);
void mliScenery_free(struct mliScenery *scenery);
void _mliScenery_free_user_ids(struct mliScenery *scenery);
struct mliScenery mliScenery_init(void);
struct mliSceneryCapacity mliSceneryCapacity_init(void);
#endif
