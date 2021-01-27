/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RAY_OCTREE_TRAVERSAL_H_
#define MERLICT_C89_MLI_RAY_OCTREE_TRAVERSAL_H_

#include <stdint.h>
#include <float.h>

#include "mliOcTree.h"
#include "mliCube.h"
#include "mli_math.h"

void mli_ray_octree_traversal(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mliRay ray,
        struct mliIntersection *isec);

void _mli_ray_octree_traversal(
        const struct mliOcTree *octree,
        const struct mliRay ray,
        void *work,
        void (*work_on_leaf_node)(void *, const struct mliOcTree *, const uint32_t));

void _mli_proc_subtree(
        struct mliVec t0,
        struct mliVec t1,
        const struct mliOcTree *octree,
        const int32_t node_idx,
        const int32_t node_type,
        uint8_t permutation,
        void *work,
        void (*work_on_leaf_node)(void *, const struct mliOcTree *, const uint32_t));

int _mli_next_octree_node(const struct mliVec tm, int x, int y, int z);

int _mli_first_octree_node(const struct mliVec t0, const struct mliVec tm);

/*
void mli_set_txm_tym_tzm(
        const struct mliVec t0,
        const struct mliVec t1,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliVec *tm);


void _mli_proc_subtree(
        struct mliVec t0,
        struct mliVec t1,
        const struct mliOcTree *octree,
        const int32_t octree_node,
        const int32_t octree_type,
        uint8_t permutation,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliIntersection *isec,
        const struct mliScenery *scenery,
        const struct mliRay ray);
*/

#endif
