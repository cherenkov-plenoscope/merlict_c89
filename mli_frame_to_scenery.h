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
                mliFrame* child = mliFrame_child(frame, c);
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
        default: mli_sentinel("Unknown type of frame."); break;}
    return 1;
error:
    return 0;
}

int mliScenery_malloc_from_mliUserScenery(
    mliScenery* scenery,
    mliUserScenery* uscn) {
    uint64_t i;

    /* transformations */
    mliFrame_set_frame2root(&uscn->root);

    /* capacity of surfaces */
    scenery->num_functions = uscn->surface_resources.num_functions;
    scenery->num_colors = uscn->surface_resources.num_colors;
    scenery->num_surfaces = uscn->surface_resources.num_surfaces;

    /* capacity of primitives */
    mli_check(
        __mliScenery_set_primitive_capacity(scenery, &uscn->root),
        "Can not estimate capacity of primitives.");

    /* malloc scenery */
    mli_check(
        mliScenery_malloc(scenery),
        "Can not allocate scenery.");

    /* copy surfaces */
    for (i = 0; i < scenery->num_functions; i++) {
        scenery->functions[i].num_points =
            uscn->surface_resources.functions[i].num_points;
        mli_check(
            mliFunc_malloc(&scenery->functions[i]),
            "Failed to allocate mliFunc in mliScenery.");
        mli_check(
            mliFunc_cpy(
                &scenery->functions[i],
                &uscn->surface_resources.functions[i]),
            "Failed to copy function to mliScenery.");
    }
    for (i = 0; i < scenery->num_colors; i++) {
        scenery->colors[i] = uscn->surface_resources.colors[i];}
    for (i = 0; i < scenery->num_surfaces; i++) {
        scenery->surfaces[i] = uscn->surface_resources.surfaces[i];}

    return 1;
error:
    return 0;
}

#endif
