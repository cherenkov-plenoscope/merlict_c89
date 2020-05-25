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
#include "mliBiCirclePlane_OBB.h"
#include "mliBiCirclePlane_intersection.h"
#include "mliDisc_OBB.h"
#include "mliDisc_intersection.h"
#include "mliOBB.h"
#include "mliIntersection.h"

struct mliIndex {
        uint32_t type;
        uint32_t idx;
};

struct mliIndexStarts {
        uint64_t triangles;
        uint64_t spherical_cap_hex;
        uint64_t spheres;
        uint64_t cylinders;
        uint64_t hexagons;
        uint64_t bicircleplanes;
        uint64_t discs;
        uint64_t _next;
};

struct mliIndexStarts mliScenery_index_starts(const struct mliScenery *scenery)
{
        struct mliIndexStarts s;
        s.triangles = 0;
        s.spherical_cap_hex = scenery->num_triangles;
        s.spheres = s.spherical_cap_hex + scenery->num_spherical_cap_hex;
        s.cylinders = s.spheres + scenery->num_spheres;
        s.hexagons = s.cylinders + scenery->num_cylinders;
        s.bicircleplanes = s.hexagons + scenery->num_hexagons;
        s.discs = s.bicircleplanes + scenery->num_bicircleplanes;
        s._next = s.discs + scenery->num_discs;
        return s;
}

uint64_t mliScenery_num_primitives(const struct mliScenery *scenery)
{
        struct mliIndexStarts starts = mliScenery_index_starts(scenery);
        return starts._next;
}

struct mliIndex _mliScenery_resolve_index(
        const struct mliScenery *scenery,
        const uint64_t idx)
{
        struct mliIndexStarts starts = mliScenery_index_starts(scenery);

        struct mliIndex ri;
        if (idx < scenery->num_triangles) {
                ri.type = MLI_TRIANGLE;
                ri.idx = idx;
                return ri;
        } else if (idx < starts.spheres) {
                ri.type = MLI_SPHERICAL_CAP_HEX;
                ri.idx = idx - starts.spherical_cap_hex;
                return ri;
        } else if (idx < starts.cylinders) {
                ri.type = MLI_SPHERE;
                ri.idx = idx - starts.spheres;
                return ri;
        } else if (idx < starts.hexagons) {
                ri.type = MLI_CYLINDER;
                ri.idx = idx - starts.cylinders;
                return ri;
        } else if (idx < starts.bicircleplanes) {
                ri.type = MLI_HEXAGON;
                ri.idx = idx - starts.hexagons;
                return ri;
        } else if (idx < starts.discs) {
                ri.type = MLI_BICIRCLEPLANE;
                ri.idx = idx - starts.bicircleplanes;
                return ri;
        } else if (idx < starts._next) {
                ri.type = MLI_DISC;
                ri.idx = idx - starts.discs;
                return ri;
        }
        assert(idx < mliScenery_num_primitives(scenery) + 1);
        ri.type = 0u;
        ri.idx = 0u;
        return ri;
}

uint64_t _mliScenery_merge_index(
        const struct mliScenery *scenery,
        const uint32_t type,
        const uint32_t idx)
{
        struct mliIndexStarts starts = mliScenery_index_starts(scenery);
        switch (type) {
        case MLI_TRIANGLE:
                return starts.triangles + idx;
                break;
        case MLI_SPHERICAL_CAP_HEX:
                return starts.spherical_cap_hex + idx;
                break;
        case MLI_SPHERE:
                return starts.spheres + idx;
                break;
        case MLI_CYLINDER:
                return starts.cylinders + idx;
                break;
        case MLI_HEXAGON:
                return starts.hexagons + idx;
                break;
        case MLI_BICIRCLEPLANE:
                return starts.bicircleplanes + idx;
                break;
        case MLI_DISC:
                return starts.discs + idx;
                break;
        default:
                return 0;
                break;
        }
        return 0;
}

int mliScenery_overlap_obb(
        const struct mliScenery *scenery,
        const uint64_t idx,
        const struct mliOBB obb)
{
        struct mliIndex i = _mliScenery_resolve_index(scenery, idx);
        switch (i.type) {
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
                        scenery->spheres[i.idx].radius,
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
        case MLI_BICIRCLEPLANE:
                return mliBiCirclePlane_has_overlap_obb(
                        scenery->bicircleplanes[i.idx],
                        scenery->bicircleplanes_T[i.idx],
                        obb);
                break;
        case MLI_DISC:
                return mliDisc_has_overlap_obb(
                        scenery->discs[i.idx], scenery->discs_T[i.idx], obb);
                break;
        default:
                return 0;
                break;
        }
        return 0;
}

struct mliOBB mliScenery_obb(
        const struct mliScenery *scenery,
        const uint64_t idx)
{
        struct mliOBB obb;
        struct mliIndex i = _mliScenery_resolve_index(scenery, idx);
        switch (i.type) {
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
                        scenery->spheres[i.idx].radius,
                        scenery->spheres_T[i.idx].trans);
                break;
        case MLI_CYLINDER:
                return mliCylinder_obb(
                        scenery->cylinders[i.idx], scenery->cylinders_T[i.idx]);
                break;
        case MLI_HEXAGON:
                return mliHexagon_obb(
                        scenery->hexagons[i.idx], scenery->hexagons_T[i.idx]);
                break;
        case MLI_BICIRCLEPLANE:
                return mliBiCirclePlane_obb(
                        scenery->bicircleplanes[i.idx],
                        scenery->bicircleplanes_T[i.idx]);
                break;
        case MLI_DISC:
                return mliDisc_obb(
                        scenery->discs[i.idx], scenery->discs_T[i.idx]);
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

struct mliOBB mliScenery_outermost_obb(const struct mliScenery *scenery)
{
        uint64_t i;
        uint64_t num_primitives;
        struct mliOBB obb;
        obb.lower = mliVec_set(0., 0., 0.);
        obb.upper = mliVec_set(0., 0., 0.);
        num_primitives = mliScenery_num_primitives(scenery);
        for (i = 0; i < num_primitives; i++) {
                struct mliOBB obj_obb = mliScenery_obb(scenery, i);
                obb = mliOBB_outermost(obb, obj_obb);
        }
        return obb;
}

int mliScenery_intersection(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const uint64_t idx,
        struct mliIntersection *intersection)
{
        struct mliIndex i = _mliScenery_resolve_index(scenery, idx);
        intersection->object_idx = idx;
        switch (i.type) {
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
        case MLI_BICIRCLEPLANE:
                return mliBiCirclePlane_intersection(
                        scenery->bicircleplanes[i.idx],
                        scenery->bicircleplanes_T[i.idx],
                        ray,
                        intersection);
                break;
        case MLI_DISC:
                return mliDisc_intersection(
                        scenery->discs[i.idx],
                        scenery->discs_T[i.idx],
                        ray,
                        intersection);
                break;
        default:
                return 0;
                break;
        }
        return 0;
}

struct mliBoundaryLayer mliScenery_object_surfaces(
        const struct mliScenery *scenery,
        const uint64_t idx)
{
        struct mliBoundaryLayer null_layer;
        struct mliIndex i = _mliScenery_resolve_index(scenery, idx);
        switch (i.type) {
        case MLI_TRIANGLE:
                return scenery->triangles_boundary_layers[i.idx];
                break;
        case MLI_SPHERICAL_CAP_HEX:
                return scenery->spherical_cap_hex_boundary_layers[i.idx];
                break;
        case MLI_SPHERE:
                return scenery->spheres_boundary_layers[i.idx];
                break;
        case MLI_CYLINDER:
                return scenery->cylinders_boundary_layers[i.idx];
                break;
        case MLI_HEXAGON:
                return scenery->hexagons_boundary_layers[i.idx];
                break;
        case MLI_BICIRCLEPLANE:
                return scenery->bicircleplanes_boundary_layers[i.idx];
                break;
        case MLI_DISC:
                return scenery->discs_boundary_layers[i.idx];
                break;
        }
        null_layer.inner.surface = 0;
        null_layer.outer.surface = 0;
        null_layer.inner.medium = 0;
        null_layer.outer.medium = 0;
        return null_layer;
}

#endif
