/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIACCELERATOR_H_
#define MLIACCELERATOR_H_

#include <stdint.h>
#include "mliGeometry.h"
#include "mliOcTree.h"

struct mliAccelerator {
        uint32_t num_objects;
        struct mliOcTree *object_octrees;

        uint32_t num_robjects;
        struct mliAABB *robject_aabbs;

        struct mliOcTree scenery_octree;
};

struct mliAccelerator mliAccelerator_init(void);

void mliAccelerator_free(struct mliAccelerator *accel);

int mliAccelerator_malloc(
        struct mliAccelerator *accel,
        const uint32_t num_objects,
        const uint32_t num_robjects);

int mliAccelerator_malloc_from_Geometry(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry);

int mliAccelerator_set_robject_aabbs(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry);

int mliAccelerator_set_object_octrees(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry);

void mliAccelerator_info_fprint(FILE *f, const struct mliAccelerator *accel);

struct mliAABB mliAccelerator_outermost_aabb(
        const struct mliAccelerator *accel);

#endif
