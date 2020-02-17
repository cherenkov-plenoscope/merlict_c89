/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_FRAME_TO_SCENERY_H_
#define MERLICT_FRAME_TO_SCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliUserScenery.h"

struct mliPrimitiveCount {
    uint32_t vertices;
    uint32_t triangles;
    uint32_t spherical_cap_hex;
    uint32_t spheres;
    uint32_t cylinders;
    uint32_t hexagons;
    uint32_t bicircleplanes;
    uint32_t discs;
};

struct mliPrimitiveCount mliPrimitiveCount_init() {
    struct mliPrimitiveCount i;
    i.vertices = 0;
    i.triangles = 0;
    i.spherical_cap_hex = 0;
    i.spheres = 0;
    i.cylinders = 0;
    i.hexagons = 0;
    i.bicircleplanes = 0;
    i.discs = 0;
    return i;}

int __mliScenery_set_primitive_capacity(
    mliScenery* scenery,
    const mliFrame* frame) {
    uint64_t c;
    switch (frame->type) {
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
    return 0;}

int __mliScenery_set_primitive(
    mliScenery* scenery,
    const mliFrame* frame,
    struct mliPrimitiveCount *count)
{
    uint64_t i, vertex_offset;
    mliFrame* child;
    struct mliVec vertex_in_root;
    struct mliVec vertex;
    struct mliFace triangle;
    struct mliHomTra frame2root;
    switch (frame->type) {
        case MLI_FRAME:
            for (i = 0; i < frame->children.size; i++) {
                child = mliFrame_child(frame, i);
                __mliScenery_set_primitive(scenery, child, count);}
            break;
        case MLI_MESH:
            vertex_offset = count->vertices;
            frame2root = mliHomTra_from_compact(frame->frame2root);
            for (i = 0; i < frame->primitive.mesh->num_vertices; i++) {
                vertex = frame->primitive.mesh->vertices[i];
                vertex_in_root = mliHomTra_pos(&frame2root, vertex);
                scenery->vertices[count->vertices] = vertex_in_root;
                count->vertices += 1;}
            for (i = 0; i < frame->primitive.mesh->num_faces; i++) {
                triangle = frame->primitive.mesh->faces[i];
                triangle.a += vertex_offset;
                triangle.b += vertex_offset;
                triangle.c += vertex_offset;
                scenery->triangles[count->triangles] = triangle;
                scenery->triangles_surfaces[count->triangles] = frame->surfaces;
                count->triangles += 1;}
            break;
        case MLI_SPHERICAL_CAP_HEX:
            i = count->spherical_cap_hex;
            scenery->spherical_cap_hex[i] = *frame->primitive.spherical_cap_hex;
            scenery->spherical_cap_hex_surfaces[i] = frame->surfaces;
            scenery->spherical_cap_hex_T[i] = frame->frame2root;
            count->spherical_cap_hex += 1;
            break;
        case MLI_SPHERE:
            i = count->spheres;
            scenery->spheres[i] = *frame->primitive.sphere;
            scenery->spheres_surfaces[i] = frame->surfaces;
            scenery->spheres_T[i] = frame->frame2root;
            count->spheres += 1;
            break;
        case MLI_CYLINDER:
            i = count->cylinders;
            scenery->cylinders[i] = *frame->primitive.cylinder;
            scenery->cylinders_surfaces[i] = frame->surfaces;
            scenery->cylinders_T[i] = frame->frame2root;
            count->cylinders += 1;
            break;
        case MLI_HEXAGON:
            i = count->hexagons;
            scenery->hexagons[i] = *frame->primitive.hexagon;
            scenery->hexagons_surfaces[i] = frame->surfaces;
            scenery->hexagons_T[i] = frame->frame2root;
            count->hexagons += 1;
            break;
        case MLI_BICIRCLEPLANE:
            i = count->bicircleplanes;
            scenery->bicircleplanes[i] = *frame->primitive.bicircleplane;
            scenery->bicircleplanes_surfaces[i] = frame->surfaces;
            scenery->bicircleplanes_T[i] = frame->frame2root;
            count->bicircleplanes += 1;
            break;
        case MLI_DISC:
            i = count->discs;
            scenery->discs[i] = *frame->primitive.disc;
            scenery->discs_surfaces[i] = frame->surfaces;
            scenery->discs_T[i] = frame->frame2root;
            count->discs += 1;
            break;
        default: mli_sentinel("Unknown type of frame."); break;}
    return 1;
error:
    return 0;}

int mliScenery_malloc_from_mliUserScenery(
    mliScenery* scenery,
    mliUserScenery* uscn) {

    uint64_t i;
    struct mliPrimitiveCount count = mliPrimitiveCount_init();

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
            "Failed to allocate struct mliFunc in mliScenery.");
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

    mli_check(
        __mliScenery_set_primitive(scenery, &uscn->root, &count),
        "Cen not set primitives");

    return 1;
error:
    return 0;
}

int mliScenery_malloc_from_json_path(mliScenery* scenery, const char* path) {
    struct mliJson json = mliJson_init();
    mliUserScenery uscn = mliUserScenery_init();
    mli_check(
        mliJson_malloc_from_file(&json, path),
        "Failed to parse json-scenery.");
    mli_check(
        mliUserScenery_malloc_from_json(&uscn, &json),
        "Failed to parse json-scenery.");
    mli_check(
        mliScenery_malloc_from_mliUserScenery(scenery, &uscn),
        "Failed to translate mliUserScenery to mliScenery.")
    mliJson_free(&json);
    mliUserScenery_free(&uscn);
    return 1;
error:
    mliJson_free(&json);
    mliUserScenery_free(&uscn);
    return 0;}

#endif
