/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAY_SCENERY_QUERY_H_
#define MLI_RAY_SCENERY_QUERY_H_

#include <stdint.h>
#include "mli_ray_octree_traversal.h"
#include "../ray/ray.h"
#include "../homtra/homtra.h"
#include "../object/object.h"
#include "../octree/octree.h"
#include "../accelerator/accelerator.h"
#include "mliScenery.h"
#include "mliIntersection.h"
#include "mliIntersectionSurfaceNormal.h"

int mli_query_intersection(
        const struct mliScenery *scenery,
        const struct mli_Ray ray_root,
        struct mliIntersection *isec);

int mli_query_intersection_with_surface_normal(
        const struct mliScenery *scenery,
        const struct mli_Ray ray_root,
        struct mliIntersectionSurfaceNormal *isecsrf);

int mli_query_object_reference(
        const struct mli_Object *object,
        const struct mli_OcTree *object_octree,
        const struct mli_HomTraComp local2root_comp,
        const struct mli_Ray ray_root,
        struct mliIntersection *isec);

struct mliQueryInnerWork {
        struct mliIntersection *intersection;
        const struct mli_Object *object;
        struct mli_Ray ray_object;
        int has_intersection;
};

struct mliQueryOuterWork {
        struct mliIntersection *intersection;
        const struct mli_Geometry *geometry;
        const struct mliAccelerator *accelerator;
        struct mli_Ray ray_root;
};

void mli_outer_scenery_traversal(
        void *_outer,
        const struct mli_OcTree *scenery_octree,
        const uint32_t scenery_octree_leaf_idx);

void mli_inner_object_traversal(
        void *_inner,
        const struct mli_OcTree *object_octree,
        const uint32_t object_octree_leaf_idx);

#endif
