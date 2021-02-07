/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RAY_SCENERY_QUERY_H_
#define MERLICT_C89_MLI_RAY_SCENERY_QUERY_H_

#include "mli_ray_octree_traversal.h"
#include "mliCombine.h"

void mli_first_casual_presection(
        const struct mliCombine *combine,
        const struct mliRay ray_wrt_root,
        struct mliIntersectionMinimalQuery *presection);

int mli_first_casual_intersection(
        const struct mliCombine *combine,
        const struct mliRay ray,
        struct mliIntersectionSurfaceNormal *intersection);

int _mliRobject_first_causual_intersection(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray_wrt_root,
        struct mliIntersectionMinimalQuery *presection);

#endif
