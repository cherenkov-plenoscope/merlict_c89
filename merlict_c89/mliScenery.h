/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_H_
#define MERLICT_C89_MLISCENERY_H_

#include <stdint.h>
#include "mliHomTra.h"
#include "mliBoundaryLayer.h"
#include "mliSceneryResources.h"

struct mliScenery {
        struct mliSceneryResources resources;

        uint32_t num_robjects;
        uint32_t *robjects;
        uint32_t *robject_ids;
        struct mliBoundaryLayer *robject_boundary_layers;
        struct mliHomTraComp *robject2root;
};

int mliScenery_malloc(struct mliScenery *scenery, const uint32_t num_robjects);
void mliScenery_free(struct mliScenery *scenery);
struct mliScenery mliScenery_init(void);
void mliScenery_info_fprint(FILE *f, const struct mliScenery *scenery);
struct mliBoundaryLayer mliScenery_object_surfaces(
        const struct mliScenery *scenery,
        const uint32_t object_idx);
#endif
