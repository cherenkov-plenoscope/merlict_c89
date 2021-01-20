/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIACCELERATOR_H_
#define MERLICT_C89_MLIACCELERATOR_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliOcTree.h"

struct mliAccelerator {
        uint32_t num_objects;
        struct mliOcTree *object_octrees;

        uint32_t num_robjects;
        struct mliOBB *robject_obbs;

        struct mliOcTree scenery_octree;
};

struct mliAccelerator mliAccelerator_init(void);

void mliAccelerator_free(struct mliAccelerator *accel);

int mliAccelerator_malloc(
        struct mliAccelerator *accel,
        const uint32_t num_objects,
        const uint32_t num_robjects);

int mliAccelerator_malloc_from_scenery(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery);

int _mliAccelerator_set_robject_obbs(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery);

int _mliAccelerator_set_object_octrees(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery);

void mliAccelerator_fprint(FILE *f, const struct mliAccelerator *accel);

/*
int mliAccelerator_robject_has_overlap_obb(
        const struct mliAccelerator *accel,
        const uint64_t robject_idx,
        const struct mliOBB obb);
*/

#endif
