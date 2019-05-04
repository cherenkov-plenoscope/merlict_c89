/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIINDEX_H_
#define MERLICT_MLIINDEX_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliVec.h"
#include "mliTriangle_OBB.h"
#include "mliSphericalCapHex_OBB.h"

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
    const mliVec obb_lower,
    const mliVec obb_upper) {
    /* aabox is axis aligned box */
    mliIndex i = __mliScenery_resolve_index(scenery, idx);

    switch(i.type) {
        case MLI_TRIANGLE:
            return mliTriangle_overlap_obb(
                scenery->vertices[scenery->triangles[i.idx].a],
                scenery->vertices[scenery->triangles[i.idx].b],
                scenery->vertices[scenery->triangles[i.idx].c],
                obb_lower,
                obb_upper);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            return mliSphericalCapHex_overlap_obb(
                scenery->spherical_cap_hex[i.idx],
                scenery->spherical_cap_hex_T[i.idx],
                obb_lower,
                obb_upper);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

#endif
