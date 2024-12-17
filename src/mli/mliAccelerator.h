/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIACCELERATOR_H_
#define MLIACCELERATOR_H_

#include <stdint.h>
#include "../geometry/geometry.h"
#include "../octree/octree.h"

struct mliAccelerator {
        uint32_t num_objects;
        struct mli_OcTree *object_octrees;

        uint32_t num_robjects;
        struct mli_AABB *robject_aabbs;

        struct mli_OcTree scenery_octree;
};

struct mliAccelerator mliAccelerator_init(void);

void mliAccelerator_free(struct mliAccelerator *accel);

int mliAccelerator_malloc(
        struct mliAccelerator *accel,
        const uint32_t num_objects,
        const uint32_t num_robjects);

int mliAccelerator_malloc_from_Geometry(
        struct mliAccelerator *accel,
        const struct mli_Geometry *geometry);

int mliAccelerator_set_robject_aabbs(
        struct mliAccelerator *accel,
        const struct mli_Geometry *geometry);

int mliAccelerator_set_object_octrees(
        struct mliAccelerator *accel,
        const struct mli_Geometry *geometry);

void mliAccelerator_info_fprint(FILE *f, const struct mliAccelerator *accel);

struct mli_AABB mliAccelerator_outermost_aabb(
        const struct mliAccelerator *accel);

#endif
