/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIINDEX_H_
#define MERLICT_MLIINDEX_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliVec.h"
#include "mliTriangle_OBB.h"
#include "mliSphere_OBB.h"
#include "mliSphere_intersection.h"
#include "mliSphericalCapHex_OBB.h"
#include "mliSphericalCapHex_intersection.h"
#include "mliOBB.h"
#include "mliIntersection.h"

typedef struct {
    uint32_t type;
    uint32_t idx;
} mliIndex;

uint64_t mliScenery_num_entities(const mliScenery* scenery) {
    uint64_t last = 0;
    last += scenery->num_triangles;
    last += scenery->num_spherical_cap_hex;
    last += scenery->num_spheres;
    return last;}

mliIndex __mliScenery_resolve_index(
    const mliScenery* scenery,
    const uint64_t idx) {
    const uint64_t idx_start_spherical_cap_hex =
        scenery->num_triangles;
    const uint64_t idx_start_spheres =
        idx_start_spherical_cap_hex + scenery->num_spherical_cap_hex;
    const uint64_t idx_next =
        idx_start_spheres + scenery->num_spheres;
    mliIndex ri;
    if (idx < scenery->num_triangles) {
        ri.type = MLI_TRIANGLE;
        ri.idx = idx;
        return ri;
    } else if (idx < idx_start_spheres) {
        ri.type = MLI_SPHERICAL_CAP_HEX;
        ri.idx = idx - idx_start_spherical_cap_hex;
        return ri;
    } else if (idx < idx_next) {
        ri.type = MLI_SPHERE;
        ri.idx = idx - idx_start_spheres;
        return ri;
    }
    assert(idx < mliScenery_num_entities(scenery) + 1);
    ri.type = 0u;
    ri.idx = 0u;
    return ri;}

int mliScenery_overlap_obb(
    const mliScenery* scenery,
    const uint64_t idx,
    const mliOBB obb) {
    mliIndex i = __mliScenery_resolve_index(scenery, idx);
    switch(i.type) {
        case MLI_TRIANGLE:
            return mliTriangle_has_overlap_obb(
                scenery->vertices[scenery->triangles[i.idx].a],
                scenery->vertices[scenery->triangles[i.idx].b],
                scenery->vertices[scenery->triangles[i.idx].c],
                obb);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return mliSphericalCapHex_has_overlap_obb(
                scenery->spherical_cap_hex[i.idx],
                scenery->spherical_cap_hex_T[i.idx],
                obb);
            break;
        case MLI_SPHERE:
            return mliSphere_has_overlap_obb(
                scenery->spheres[i.idx],
                scenery->spheres_T[i.idx].trans,
                obb);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

mliOBB mliScenery_obb(
    const mliScenery* scenery,
    const uint64_t idx) {
    mliOBB obb;
    mliIndex i = __mliScenery_resolve_index(scenery, idx);
    switch(i.type) {
        case MLI_TRIANGLE:
            return mliTriangle_obb(
                scenery->vertices[scenery->triangles[i.idx].a],
                scenery->vertices[scenery->triangles[i.idx].b],
                scenery->vertices[scenery->triangles[i.idx].c]);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return mliSphericalCapHex_obb(
                scenery->spherical_cap_hex[i.idx],
                scenery->spherical_cap_hex_T[i.idx]);
            break;
        case MLI_SPHERE:
            return mliSphere_obb(
                scenery->spheres[i.idx],
                scenery->spheres_T[i.idx].trans);
            break;
        default:
            obb.lower = mliVec_set(0., 0., 0);
            obb.upper = mliVec_set(0., 0., 0);
            return obb;
            break;
    }
    obb.lower = mliVec_set(0., 0., 0);
    obb.upper = mliVec_set(0., 0., 0);
    return obb;
}

mliOBB mliScenery_outermost_obb(const mliScenery *scenery) {
    uint64_t i;
    uint64_t num_objects;
    mliOBB obb;
    obb.lower = mliVec_set(0., 0., 0.);
    obb.upper = mliVec_set(0., 0., 0.);
    num_objects = mliScenery_num_entities(scenery);
    for (i = 0; i < num_objects; i++) {
        mliOBB obj_obb = mliScenery_obb(scenery, i);
        obb = mliOBB_outermost(obb, obj_obb);
    }
    return obb;
}

int mliScenery_intersection(
    const mliScenery* scenery,
    const mliRay ray,
    const uint64_t idx,
    mliIntersection *intersection) {
    mliIndex i = __mliScenery_resolve_index(scenery, idx);
    intersection->object_idx = idx;
    switch(i.type) {
        case MLI_TRIANGLE:
            return mliTriangle_intersection(
                scenery->vertices[scenery->triangles[i.idx].a],
                scenery->vertices[scenery->triangles[i.idx].b],
                scenery->vertices[scenery->triangles[i.idx].c],
                ray,
                intersection);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return mliSphericalCapHex_intersection(
                scenery->spherical_cap_hex[i.idx],
                scenery->spherical_cap_hex_T[i.idx],
                ray,
                intersection);
            break;
        case MLI_SPHERE:
            return mliSphere_intersection(
                scenery->spheres[i.idx],
                scenery->spheres_T[i.idx],
                ray,
                intersection);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

#endif
