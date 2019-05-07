/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIINDEX_H_
#define MERLICT_MLIINDEX_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliVec.h"
#include "mliTriangle_OBB.h"
#include "mliSphericalCapHex_OBB.h"
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
    return last;}

mliIndex __mliScenery_resolve_index(
    const mliScenery* scenery,
    const uint64_t idx) {
    mliIndex ri;
    if (idx < scenery->num_triangles) {
        ri.type = MLI_TRIANGLE;
        ri.idx = idx;
        return ri;
    } else if (idx < scenery->num_triangles + scenery->num_spherical_cap_hex) {
        ri.type = MLI_SPHERICAL_CAP_HEX;
        ri.idx = idx - scenery->num_triangles;
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
            return mliTriangle_overlap_obb(
                scenery->vertices[scenery->triangles[i.idx].a],
                scenery->vertices[scenery->triangles[i.idx].b],
                scenery->vertices[scenery->triangles[i.idx].c],
                obb);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return mliSphericalCapHex_overlap_obb(
                scenery->spherical_cap_hex[i.idx],
                scenery->spherical_cap_hex_T[i.idx],
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
    intersection->idx_tri = idx;
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
        default:
            return 0;
            break;
    }
    return 0;
}

#endif
