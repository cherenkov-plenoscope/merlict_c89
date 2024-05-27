/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_RAY_GRID_TRAVERSAL_H_
#define MLI_RAY_GRID_TRAVERSAL_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliRay_AABB.h"

struct mliIdx3 {
        int64_t x;
        int64_t y;
        int64_t z;
};

struct mliIdx3 mliIdx3_set(const int64_t x, const int64_t y, const int64_t z);


struct mliAxisAlignedGrid {
        struct mliAABB bounds;
        struct mliIdx3 num_bins;
        struct mliVec bin_width;
};

struct mliAxisAlignedGrid mliAxisAlignedGrid_set(
        struct mliAABB bounds,
        struct mliIdx3 num_bins);

int mliAxisAlignedGrid_find_voxel_of_first_interaction(
        const struct mliAxisAlignedGrid* grid,
        const struct mliRay* ray,
        struct mliIdx3* bin);

#define MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID 0
#define MLI_AXIS_ALIGNED_GRID_RAY_STARTS_INSIDE_GRID 1
#define MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS 2

struct mliAxisAlignedGridTraversal {
        const struct mliAxisAlignedGrid* grid;
        struct mliIdx3 voxel;
        struct mliVec step;
        struct mliVec tMax;
        struct mliVec tDelta;
        int valid;
};

struct mliAxisAlignedGridTraversal mliAxisAlignedGridTraversal_start(
        const struct mliAxisAlignedGrid* grid,
        const struct mliRay* ray);
int mliAxisAlignedGridTraversal_next(struct mliAxisAlignedGridTraversal* traversal);

void mliAxisAlignedGridTraversal_fprint(FILE* f, struct mliAxisAlignedGridTraversal* traversal);

#endif
