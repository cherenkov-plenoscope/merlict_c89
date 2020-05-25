/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_FRAME_TO_SCENERY_H_
#define MERLICT_FRAME_TO_SCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliScenery.h"
#include "mliUserScenery.h"
#include "mliPrimitiveIdMap.h"

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

struct mliPrimitiveCount mliPrimitiveCount_init(void)
{
        struct mliPrimitiveCount i;
        i.vertices = 0;
        i.triangles = 0;
        i.spherical_cap_hex = 0;
        i.spheres = 0;
        i.cylinders = 0;
        i.hexagons = 0;
        i.bicircleplanes = 0;
        i.discs = 0;
        return i;
}

int __mliScenery_set_primitive_capacity(
        struct mliSceneryCapacity *scn_cap,
        const struct mliFrame *frame)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        struct mliFrame *child = frame->children.arr[c];
                        mli_c(__mliScenery_set_primitive_capacity(
                                scn_cap, child));
                }
                break;
        case MLI_MESH:
                scn_cap->num_vertices += frame->primitive.mesh->num_vertices;
                scn_cap->num_triangles += frame->primitive.mesh->num_faces;
                break;
        case MLI_SPHERICAL_CAP_HEX:
                scn_cap->num_spherical_cap_hex++;
                break;
        case MLI_SPHERE:
                scn_cap->num_spheres++;
                break;
        case MLI_CYLINDER:
                scn_cap->num_cylinders++;
                break;
        case MLI_HEXAGON:
                scn_cap->num_hexagons++;
                break;
        case MLI_BICIRCLEPLANE:
                scn_cap->num_bicircleplanes++;
                break;
        case MLI_DISC:
                scn_cap->num_discs++;
                break;
        default:
                mli_sentinel("Unknown type of frame.");
                break;
        }
        return 1;
error:
        return 0;
}

int __mliScenery_set_primitive(
        struct mliScenery *scenery,
        const struct mliFrame *frame,
        struct mliPrimitiveCount *count,
        struct mliPrimitiveIdMap *idmap)
{
        uint64_t i, vertex_offset, primitive_idx;
        struct mliFrame *child;
        struct mliVec vertex_in_root;
        struct mliVec vertex;
        struct mliFace triangle;
        struct mliHomTra frame2root;
        switch (frame->type) {
        case MLI_FRAME:
                for (i = 0; i < frame->children.dyn.size; i++) {
                        child = frame->children.arr[i];
                        __mliScenery_set_primitive(
                                scenery,
                                child,
                                count,
                                idmap);
                }
                break;
        case MLI_MESH:
                vertex_offset = count->vertices;
                frame2root = mliHomTra_from_compact(frame->frame2root);
                for (i = 0; i < frame->primitive.mesh->num_vertices; i++) {
                        vertex = frame->primitive.mesh->vertices[i];
                        vertex_in_root = mliHomTra_pos(&frame2root, vertex);
                        scenery->vertices[count->vertices] = vertex_in_root;
                        count->vertices += 1;
                }
                for (i = 0; i < frame->primitive.mesh->num_faces; i++) {
                        primitive_idx = _mliScenery_merge_index(
                                scenery,
                                MLI_TRIANGLE,
                                count->triangles);
                        idmap->user_ids[primitive_idx] = frame->id;

                        triangle = frame->primitive.mesh->faces[i];
                        triangle.a += vertex_offset;
                        triangle.b += vertex_offset;
                        triangle.c += vertex_offset;
                        scenery->triangles[count->triangles] = triangle;
                        scenery->triangles_boundary_layers[count->triangles] =
                                frame->boundary_layer;
                        count->triangles += 1;
                }
                break;
        case MLI_SPHERICAL_CAP_HEX:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_SPHERICAL_CAP_HEX,
                        count->spherical_cap_hex);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->spherical_cap_hex;
                scenery->spherical_cap_hex[i] =
                        *frame->primitive.spherical_cap_hex;
                scenery->spherical_cap_hex_boundary_layers[i] =
                        frame->boundary_layer;
                scenery->spherical_cap_hex_T[i] = frame->frame2root;
                count->spherical_cap_hex += 1;
                break;
        case MLI_SPHERE:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_SPHERE,
                        count->spheres);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->spheres;
                scenery->spheres[i] = *frame->primitive.sphere;
                scenery->spheres_boundary_layers[i] = frame->boundary_layer;
                scenery->spheres_T[i] = frame->frame2root;
                count->spheres += 1;
                break;
        case MLI_CYLINDER:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_CYLINDER,
                        count->cylinders);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->cylinders;
                scenery->cylinders[i] = *frame->primitive.cylinder;
                scenery->cylinders_boundary_layers[i] = frame->boundary_layer;
                scenery->cylinders_T[i] = frame->frame2root;
                count->cylinders += 1;
                break;
        case MLI_HEXAGON:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_HEXAGON,
                        count->hexagons);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->hexagons;
                scenery->hexagons[i] = *frame->primitive.hexagon;
                scenery->hexagons_boundary_layers[i] = frame->boundary_layer;
                scenery->hexagons_T[i] = frame->frame2root;
                count->hexagons += 1;
                break;
        case MLI_BICIRCLEPLANE:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_BICIRCLEPLANE,
                        count->bicircleplanes);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->bicircleplanes;
                scenery->bicircleplanes[i] = *frame->primitive.bicircleplane;
                scenery->bicircleplanes_boundary_layers[i] =
                        frame->boundary_layer;
                scenery->bicircleplanes_T[i] = frame->frame2root;
                count->bicircleplanes += 1;
                break;
        case MLI_DISC:
                primitive_idx = _mliScenery_merge_index(
                        scenery,
                        MLI_DISC,
                        count->discs);
                idmap->user_ids[primitive_idx] = frame->id;

                i = count->discs;
                scenery->discs[i] = *frame->primitive.disc;
                scenery->discs_boundary_layers[i] = frame->boundary_layer;
                scenery->discs_T[i] = frame->frame2root;
                count->discs += 1;
                break;
        default:
                mli_sentinel("Unknown type of frame.");
                break;
        }
        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_mliUserScenery(
        struct mliScenery *scenery,
        struct mliPrimitiveIdMap *idmap,
        struct mliUserScenery *uscn)
{
        uint64_t i;
        struct mliSceneryCapacity scenery_capacity = mliSceneryCapacity_init();
        struct mliSceneryResourcesCapacity resource_capacity =
                mliSceneryResourcesCapacity_init();
        struct mliPrimitiveCount count = mliPrimitiveCount_init();

        /* transformations */
        mliFrame_set_frame2root(&uscn->root);

        /* resource_capacity */
        resource_capacity.num_functions = uscn->resources.num_functions;
        resource_capacity.num_colors = uscn->resources.num_colors;
        resource_capacity.num_media = uscn->resources.num_media;
        resource_capacity.num_surfaces = uscn->resources.num_surfaces;

        /* capacity of primitives */
        mli_check(
                __mliScenery_set_primitive_capacity(
                        &scenery_capacity, &uscn->root),
                "Can not estimate capacity of primitives.");

        /* malloc scenery */
        mli_check(
                mliScenery_malloc(scenery, scenery_capacity),
                "Can not allocate scenery.");

        mli_check(
                mliSceneryResources_malloc(
                        &scenery->resources, resource_capacity),
                "Can not allocate scenery.");

        mli_check(
                mliPrimitiveIdMap_malloc(
                        idmap,
                        mliScenery_num_primitives(scenery)),
                "Can not allocate primitive user-id-map.");

        /* default_medium */
        scenery->default_medium = uscn->default_medium;

        /* copy surfaces */
        for (i = 0; i < scenery->resources.num_functions; i++) {
                mli_check(
                        mliFunc_malloc(
                                &scenery->resources.functions[i],
                                uscn->resources.functions[i].num_points),
                        "Failed to allocate struct mliFunc in mliScenery.");
                mli_check(
                        mliFunc_cpy(
                                &scenery->resources.functions[i],
                                &uscn->resources.functions[i]),
                        "Failed to copy function to mliScenery.");
        }
        for (i = 0; i < scenery->resources.num_colors; i++) {
                scenery->resources.colors[i] = uscn->resources.colors[i];
        }
        for (i = 0; i < scenery->resources.num_media; i++) {
                scenery->resources.media[i] = uscn->resources.media[i];
        }
        for (i = 0; i < scenery->resources.num_surfaces; i++) {
                scenery->resources.surfaces[i] = uscn->resources.surfaces[i];
        }

        mli_check(
                __mliScenery_set_primitive(
                        scenery,
                        &uscn->root,
                        &count,
                        idmap),
                "Cen not set primitives");

        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_json_path(
        struct mliScenery *scenery,
        struct mliPrimitiveIdMap *user_id_map,
        const char *path)
{
        struct mliJson json = mliJson_init();
        struct mliUserScenery uscn = mliUserScenery_init();
        mli_check(
                mliJson_malloc_from_file(&json, path),
                "Failed to parse json-scenery.");
        mli_check(
                mliUserScenery_malloc_from_json(&uscn, &json),
                "Failed to parse json-scenery.");
        mli_check(
                mliScenery_malloc_from_mliUserScenery(
                        scenery,
                        user_id_map,
                        &uscn),
                "Failed to translate struct mliUserScenery to mliScenery.")
                mliJson_free(&json);
        mliUserScenery_free(&uscn);
        return 1;
error:
        mliJson_free(&json);
        mliUserScenery_free(&uscn);
        return 0;
}

#endif
