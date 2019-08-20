/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_FRAME_TO_SCENERY_H_
#define MERLICT_FRAME_TO_SCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliUserScenery.h"

int __mliScenery_set_primitive_capacity(
    mliScenery* scenery,
    const mliFrame* frame) {
    uint64_t c;
    switch(frame->type) {
        case MLI_FRAME:
            for (c = 0; c < frame->children.size; c++) {
                mliFrame* child = *(
                    (mliFrame**)mliVector_at(&frame->children, c));
                mli_c(__mliScenery_set_primitive_capacity(scenery, child));
            }
            break;
        case MLI_MESH:
            scenery->num_vertices += frame->primitive.mesh->num_vertices;
            scenery->num_triangles += frame->primitive.mesh->num_faces;
            break;
        case MLI_SPHERICAL_CAP_HEX: scenery->num_spherical_cap_hex++; break;
        case MLI_SPHERE: scenery->num_spheres++; break;
        case MLI_CYLINDER: scenery->num_cylinders++; break;
        case MLI_HEXAGON: scenery->num_hexagons++; break;
        case MLI_BICIRCLEPLANE: scenery->num_bicircleplanes++; break;
        case MLI_DISC: scenery->num_discs++; break;
        default: mli_sentinel("Unknown type of primitive."); break;}
    return 1;
error:
    return 0;
}

int mliScenery_malloc_from_mliUserScenery(
    mliScenery* scenery,
    const mliUserScenery* uscn) {
    mli_check(
        uscn->root.type == MLI_FRAME,
        "Expected root to be of type FRAME.");
    mli_check(
        __mliScenery_set_primitive_capacity(scenery, &uscn->root),
        "Can not estimate capacity.");
    mli_check(
        mliScenery_malloc(scenery),
        "Can not allocate scenery.")
    return 1;
error:
    return 0;
}

#endif
