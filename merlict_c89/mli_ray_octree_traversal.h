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
void _mli_proc_subtree(
        double tx0,
        double ty0,
        double tz0,
        double tx1,
        double ty1,
        double tz1,
        const struct mliOcTree *octree,
        const int32_t octree_node,
        const int32_t octree_type,
        uint8_t a,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliIntersection *isec,
        const struct mliScenery *scenery,
        const struct mliRay ray,
        uint64_t *num_hits);
void mli_set_txm_tym_tzm(
        const struct mliVec t0,
        const struct mliVec t1,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliVec *tm);
int _mli_new_node(const struct mliVec tm, int x, int y, int z);
int _mli_first_node(
        const struct mliVec t0,
        const struct mliVec tm);
#endif
