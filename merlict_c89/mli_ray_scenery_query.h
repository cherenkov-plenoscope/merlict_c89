/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RAY_SCENERY_QUERY_H_
#define MERLICT_C89_MLI_RAY_SCENERY_QUERY_H_

#include "mli_ray_octree_traversal.h"
#include "mliCombine.h"

void mli_query_intersection_minimal(
        const struct mliCombine *combine,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isecmin);

int mli_query_intersection_with_surface_normal(
        const struct mliCombine *combine,
        const struct mliRay ray_root,
        struct mliIntersectionSurfaceNormal *isecsrf);

int _mli_query_object_reference(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isecmin);

#endif
