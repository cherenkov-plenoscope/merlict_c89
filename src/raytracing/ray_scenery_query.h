/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAYTRACING_RAY_SCENERY_QUERY_H_
#define MLI_RAYTRACING_RAY_SCENERY_QUERY_H_

#include <stdint.h>
#include "ray_octree_traversal.h"
#include "../ray/ray.h"
#include "../homtra/homtra.h"
#include "../object/object.h"
#include "../octree/octree.h"
#include "../accelerator/accelerator.h"
#include "../scenery/scenery.h"
#include "../intersection/intersection.h"
#include "../intersection/intersection_surface_normal.h"

mli_bool mli_raytracing_query_intersection(
        const struct mli_Scenery *scenery,
        const struct mli_Ray ray_root,
        struct mli_Intersection *isec);

mli_bool mli_raytracing_query_intersection_with_surface_normal(
        const struct mli_Scenery *scenery,
        const struct mli_Ray ray_root,
        struct mli_IntersectionSurfaceNormal *isecsrf);

mli_bool mli_raytracing_query_object_reference(
        const struct mli_Object *object,
        const struct mli_OcTree *object_octree,
        const struct mli_HomTraComp local2root_comp,
        const struct mli_Ray ray_root,
        struct mli_Intersection *isec);

struct mli_raytracing_QueryInnerWork {
        struct mli_Intersection *intersection;
        const struct mli_Object *object;
        struct mli_Ray ray_object;
        mli_bool has_intersection;
};

struct mli_raytracing_QueryOuterWork {
        struct mli_Intersection *intersection;
        const struct mli_Geometry *geometry;
        const struct mli_Accelerator *accelerator;
        struct mli_Ray ray_root;
};

void mli_raytracing_outer_scenery_traversal(
        void *_outer,
        const struct mli_OcTree *scenery_octree,
        const uint32_t scenery_octree_leaf_idx);

void mli_raytracing_inner_object_traversal(
        void *_inner,
        const struct mli_OcTree *object_octree,
        const uint32_t object_octree_leaf_idx);

#endif
