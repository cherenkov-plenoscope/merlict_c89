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
#include "mliCylinder_intersection.h"
#include "mliCylinder_OBB.h"
#include "mliSphericalCapHex_intersection.h"
#include "mliHexagon_OBB.h"
#include "mliHexagon_intersection.h"
#include "mliOBB.h"
#include "mliIntersection.h"

typedef struct {
    uint32_t type;
    uint32_t idx;
} mliIndex;

uint64_t mliScenery_num_objects(const mliScenery* scenery) {
    uint64_t last = 0;
    last += scenery->num_triangles;
    last += scenery->num_spherical_cap_hex;
    last += scenery->num_spheres;
    last += scenery->num_cylinders;
    last += scenery->num_hexagons;
    return last;}

mliIndex __mliScenery_resolve_index(
    const mliScenery* scenery,
    const uint64_t idx) {

    const uint64_t idx_start_spherical_cap_hex =
        scenery->num_triangles;

    const uint64_t idx_start_spheres =
        idx_start_spherical_cap_hex + scenery->num_spherical_cap_hex;

    const uint64_t idx_start_cylinders =
        idx_start_spheres + scenery->num_spheres;

    const uint64_t idx_start_hexagons =
        idx_start_cylinders + scenery->num_cylinders;

    const uint64_t idx_next =
        idx_start_hexagons + scenery->num_hexagons;

    mliIndex ri;
    if (idx < scenery->num_triangles) {
        ri.type = MLI_TRIANGLE;
        ri.idx = idx;
        return ri;
    } else if (idx < idx_start_spheres) {
        ri.type = MLI_SPHERICAL_CAP_HEX;
        ri.idx = idx - idx_start_spherical_cap_hex;
        return ri;
    } else if (idx < idx_start_cylinders) {
        ri.type = MLI_SPHERE;
        ri.idx = idx - idx_start_spheres;
        return ri;
    } else if (idx < idx_start_hexagons) {
        ri.type = MLI_CYLINDER;
        ri.idx = idx - idx_start_cylinders;
        return ri;
    } else if (idx < idx_next) {
        ri.type = MLI_HEXAGON;
        ri.idx = idx - idx_start_hexagons;
        return ri;
    }
    assert(idx < mliScenery_num_objects(scenery) + 1);
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
        case MLI_CYLINDER:
            return mliCylinder_has_overlap_obb(
                scenery->cylinders[i.idx],
                scenery->cylinders_T[i.idx],
                obb);
            break;
        case MLI_HEXAGON:
            return mliHexagon_has_overlap_obb(
                scenery->hexagons[i.idx],
                scenery->hexagons_T[i.idx],
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
        case MLI_CYLINDER:
            return mliCylinder_obb(
                scenery->cylinders[i.idx],
                scenery->cylinders_T[i.idx]);
            break;
        case MLI_HEXAGON:
            return mliHexagon_obb(
                scenery->hexagons[i.idx],
                scenery->hexagons_T[i.idx]);
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
    num_objects = mliScenery_num_objects(scenery);
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
        case MLI_CYLINDER:
            return mliCylinder_intersection(
                scenery->cylinders[i.idx],
                scenery->cylinders_T[i.idx],
                ray,
                intersection);
            break;
        case MLI_HEXAGON:
            return mliHexagon_intersection(
                scenery->hexagons[i.idx],
                scenery->hexagons_T[i.idx],
                ray,
                intersection);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

mliSurfaces mliScenery_object_surfaces(
    const mliScenery* scenery,
    const uint64_t idx) {
    mliSurfaces null = {0, 0};
    mliIndex i = __mliScenery_resolve_index(scenery, idx);
    switch(i.type) {
        case MLI_TRIANGLE:
            return scenery->triangles_surfaces[i.idx];
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return scenery->spherical_cap_hex_surfaces[i.idx];
            break;
        case MLI_SPHERE:
            return scenery->spheres_surfaces[i.idx];
            break;
        case MLI_CYLINDER:
            return scenery->cylinders_surfaces[i.idx];
            break;
        case MLI_HEXAGON:
            return scenery->hexagons_surfaces[i.idx];
            break;
    }
    return null;
}

#endif
