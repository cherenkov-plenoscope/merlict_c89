/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_TRACE_RAY_OCTREE_TRAVERSAL_H_
#define MLI_TRACE_RAY_OCTREE_TRAVERSAL_H_

#include <stdint.h>

#include "../octree/octree.h"
#include "../ray/ray.h"

#define MLI_TRACE_RAY_OCTREE_TRAVERSAL_EPSILON 1.0e-307

void mli_trace_ray_octree_traversal(
        const struct mli_OcTree *octree,
        const struct mli_Ray ray,
        void *work,
        void (*work_on_leaf_node)(
                void *,
                const struct mli_OcTree *,
                const uint32_t));

void mli_trace_ray_octree_traversal_sub(
        struct mli_Vec t0,
        struct mli_Vec t1,
        const struct mli_OcTree *octree,
        const int32_t node_idx,
        const int32_t node_type,
        uint8_t permutation,
        void *work,
        void (*work_on_leaf_node)(
                void *,
                const struct mli_OcTree *,
                const uint32_t));

int mli_trace_ray_octree_traversal_next_octree_node(
        const struct mli_Vec tm,
        int x,
        int y,
        int z);

int mli_trace_ray_octree_traversal_first_octree_node(
        const struct mli_Vec t0,
        const struct mli_Vec tm);

#endif
