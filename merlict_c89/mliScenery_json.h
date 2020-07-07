/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_JSON_H_
#define MERLICT_C89_MLISCENERY_JSON_H_

#include <stdint.h>
#include <stdlib.h>

#include "mli_debug.h"
#include "mli_json.h"
#include "mliUserScenery.h"
#include "mliFunc_malloc_from_json_token.h"
#include "mliVec_from_json_token.h"

int mliUserScenery_malloc_from_json(
        struct mliUserScenery *uscn,
        const struct mliJson *json);
int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names);
int __mliFrame_set_Mesh(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int mliFace_from_json_token(
        struct mliFace *f,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_Disc(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_BiCirclePlane(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_Hexagon(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_SphericalCapHex(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_Cylinder(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_Sphere(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_surface_idx(
        struct mliFrame *frame,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
        const struct mliJson *json,
        const uint64_t token);
int __mliSide_set(
        struct mliSide *side,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
        const struct mliJson *json,
        const uint64_t side_token);
int __mliFrame_set_id_pos_rot(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_quaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_axis_angle_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_type_from_json(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token_child);
int __mliScenery_assign_surfaces_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *surface_names,
        const struct mliMap2 *function_names,
        const struct mliMap2 *color_names,
        const struct mliMap2 *material_names,
        const struct mliJson *json);
int __mliSurface_from_json(
        struct mliSurface *surface,
        const struct mliMap2 *function_names,
        const struct mliMap2 *color_names,
        const struct mliMap2 *material_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliScenery_assign_media_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *medium_names,
        const struct mliMap2 *function_names,
        const struct mliJson *json);
int __mliMedium_from_json(
        struct mliMedium *medium,
        const struct mliMap2 *function_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliScenery_assign_colors_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *color_names,
        const struct mliJson *json);
int __mliScenery_malloc_functions_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *function_names,
        const struct mliJson *json);
int _mliMap2_get_value_for_string_from_json(
        const struct mliMap2 *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value);
int _mliMap2_key_from_json(
        struct mliMap2 *map,
        const struct mliJson *json,
        const uint64_t token_name,
        const uint64_t value);
int __mliScenery_surface_capacity_from_json(
        struct mliSceneryResourcesCapacity *res_cap,
        const struct mliJson *json);
#endif
