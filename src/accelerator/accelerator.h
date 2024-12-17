/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_H_
#define MLI_ACCELERATOR_H_

#include <stdint.h>
#include "../geometry/geometry.h"
#include "../octree/octree.h"

struct mli_Accelerator {
        uint32_t num_objects;
        struct mli_OcTree *object_octrees;

        uint32_t num_robjects;
        struct mli_AABB *robject_aabbs;

        struct mli_OcTree scenery_octree;
};

struct mli_Accelerator mli_Accelerator_init(void);

void mli_Accelerator_free(struct mli_Accelerator *self);

int mli_Accelerator_malloc(
        struct mli_Accelerator *self,
        const uint32_t num_objects,
        const uint32_t num_robjects);

int mli_Accelerator_malloc_from_Geometry(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry);

int mli_Accelerator_set_robject_aabbs(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry);

int mli_Accelerator_set_object_octrees(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry);

void mli_Accelerator_info_fprint(FILE *f, const struct mli_Accelerator *self);

struct mli_AABB mli_Accelerator_outermost_aabb(
        const struct mli_Accelerator *self);

#endif
