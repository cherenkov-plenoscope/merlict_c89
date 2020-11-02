/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_OBJECT_INTERFACE_H_
#define MERLICT_C89_MLISCENERY_OBJECT_INTERFACE_H_

#include <stdint.h>

#include "mliScenery.h"
#include "mliVec.h"
#include "mliTriangle_OBB.h"
#include "mliOBB.h"
#include "mliIntersection.h"

struct mliIndex {
        uint32_t type;
        uint32_t idx;
};

struct mliBoundaryLayer mliScenery_object_surfaces(
        const struct mliScenery *scenery,
        const uint64_t idx);
int mliScenery_intersection(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const uint64_t idx,
        struct mliIntersection *intersection);
struct mliOBB mliScenery_outermost_obb(const struct mliScenery *scenery);
struct mliOBB mliScenery_obb(
        const struct mliScenery *scenery,
        const uint64_t idx);
int mliScenery_overlap_obb(
        const struct mliScenery *scenery,
        const uint64_t idx,
        const struct mliOBB obb);
uint64_t _mliScenery_merge_index(
        const struct mliScenery *scenery,
        const uint32_t type,
        const uint32_t idx);
struct mliIndex _mliScenery_resolve_index(
        const struct mliScenery *scenery,
        const uint64_t idx);
#endif
