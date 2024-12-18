/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_AAG_H_
#define MLI_AAG_H_

#include <stdint.h>
#include "../vec/vec.h"
#include "../aabb/aabb.h"
#include "../ray/ray.h"

struct mliIdx3 {
        int64_t x;
        int64_t y;
        int64_t z;
};

struct mliIdx3 mliIdx3_set(const int64_t x, const int64_t y, const int64_t z);

struct mli_AxisAlignedGrid {
        struct mli_AABB bounds;
        struct mliIdx3 num_bins;
        struct mli_Vec bin_width;
};

struct mli_AxisAlignedGrid mli_AxisAlignedGrid_set(
        struct mli_AABB bounds,
        struct mliIdx3 num_bins);

int mli_AxisAlignedGrid_find_voxel_of_first_interaction(
        const struct mli_AxisAlignedGrid *grid,
        const struct mli_Ray *ray,
        struct mliIdx3 *bin);

#define MLI_AXISALIGNEDGRID_RAY_DOES_NOT_INTERSECT_GRID 0
#define MLI_AXISALIGNEDGRID_RAY_STARTS_INSIDE_GRID 1
#define MLI_AXISALIGNEDGRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS 2

struct mli_AxisAlignedGridTraversal {
        const struct mli_AxisAlignedGrid *grid;
        struct mliIdx3 voxel;
        struct mli_Vec step;
        struct mli_Vec tMax;
        struct mli_Vec tDelta;
        int valid;
};

struct mli_AxisAlignedGridTraversal mli_AxisAlignedGridTraversal_start(
        const struct mli_AxisAlignedGrid *grid,
        const struct mli_Ray *ray);
int mli_AxisAlignedGridTraversal_next(
        struct mli_AxisAlignedGridTraversal *traversal);

void mli_AxisAlignedGridTraversal_fprint(
        FILE *f,
        struct mli_AxisAlignedGridTraversal *traversal);
void mli_Ray_fprint(FILE *f, struct mli_Ray *ray);
#endif
